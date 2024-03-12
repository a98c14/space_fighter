#include "game.h"

#include "game.c"

#define GLYPH_METRICS_CAPACITY 128
#define FREE_GLYPH_FONT_SIZE   16

int
main(void)
{
    g_init();
    const uint32 font_size = 18;

    GameEntity* player          = g_entity_alloc();
    player->color               = ColorWhite;
    player->scale               = vec2(1, 1);
    player->speed               = 120;
    player->sprite              = SPRITE_GAME_SHIPS_RED_BEATLE;
    player->attack_rate         = 0.3;
    player->collider_type       = ColliderTypePlayerHitbox;
    player->health              = 100;
    player->collider_radius     = 20;
    player->bullet_spawn_offset = vec2(0, 18);
    g_entity_enable_prop(player, EntityProp_Player);
    g_entity_enable_prop(player, EntityProp_SmoothMovement);
    g_entity_enable_prop(player, EntityProp_RotateTowardsAim);
    g_entity_enable_prop(player, EntityProp_Collider);

    GameEntity* enemy = g_spawn_enemy(vec2(100, 100));

    /* main loop */
    while (!window_should_close(g_state->window))
    {
        arena_reset(g_state->frame_arena);
        g_state->time = engine_get_time(g_state->time);
        if (input_key_pressed(g_state->window, GLFW_KEY_RIGHT_BRACKET))
            break;

        /** gather input */
        Vec2 player_direction = vec2_zero();
        {
            if (input_key_pressed(g_state->window, GLFW_KEY_A))
            {
                player_direction.x -= 1;
            }
            if (input_key_pressed(g_state->window, GLFW_KEY_D))
            {
                player_direction.x += 1;
            }
            if (input_key_pressed(g_state->window, GLFW_KEY_W))
            {
                player_direction.y += 1;
            }
            if (input_key_pressed(g_state->window, GLFW_KEY_S))
            {
                player_direction.y -= 1;
            }
            player_direction     = norm_vec2_safe(player_direction);
            g_state->input_mouse = input_mouse_get(g_state->window, g_renderer->camera, g_state->input_mouse);
        }

        /** apply input */
        {
            player->heading = player_direction;
            player->look_at = heading_to_vec2(player->position, g_state->input_mouse.world);
            draw_heading(player->position, player->heading, ColorWhite, 2);

            if (input_mouse_held(g_state->input_mouse, MouseButtonStateLeft) && player->t_attack <= 0)
            {
                player->t_attack = player->attack_rate;

                Vec2 bullet_position = add_vec2(player->position, rotate_vec2(player->bullet_spawn_offset, player->rotation));
                g_spawn_bullet(bullet_position, player->look_at, ColliderTypePlayerAttack, ColorYellow500, 12);
                ParticleIndex p = ps_particle_animation(vec3_xy(bullet_position), ANIMATION_GAME_VFX_MUZZLE_FLASH_1, player->rotation + 90);
                post_processing_add_shake(2);
            }
        }

        float32        dt             = g_state->time.dt;
        ShaderDataText shader_data    = {0};
        shader_data.color             = d_color_white;
        shader_data.thickness         = d_default_text_thickness;
        shader_data.softness          = d_default_text_softness;
        shader_data.outline_thickness = d_default_text_outline_thickness;

        GameEntity* entity;
        /** delete marked entities */
        profiler_scope("delete marked entities") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_MarkedForDeletion))
                continue;

            bool32 drops_coin = entity->coin_on_death.min > 0 || entity->coin_on_death.max > 0;
            if (drops_coin)
            {
                uint32 coin_count = random_between_i32(entity->coin_on_death.min, entity->coin_on_death.max);
                for (uint32 i = 0; i < coin_count; i++)
                {
                    GameEntity* coin = g_entity_alloc();
                    g_entity_enable_prop(coin, EntityProp_PullTowardsPlayer);
                    coin->animation = ANIMATION_GAME_COLLECTABLES_EXPERIENCE_ORB;
                    coin->force     = random_direction(20);
                    coin->position  = entity->position;
                    coin->scale     = vec2(1, 1);
                    coin->color     = ColorWhite;
                }
            }

            g_entity_free(entity);
        }

        /** animation */
        profiler_scope("animation") for_each(entity, g_state->first_entity)
        {
            if (!(entity->animation > 0))
                continue;

            entity->t_animation_clock -= dt;

            Animation active_animation = Animations[entity->animation];
            entity->sprite             = active_animation.sprite_start_index + entity->frame;
            if (entity->t_animation_clock < 0)
            {
                entity->frame             = (entity->frame + 1) % animation_length(active_animation);
                entity->t_animation_clock = (1.0f / 24);
            }
        }

        /** attack state  */
        profiler_scope("attack state") for_each(entity, g_state->first_entity)
        {
            entity->t_attack = max(0, entity->t_attack - dt);
        }

        /** enemy spawner */
        profiler_scope("enemy spawner")
        {
            g_state->t_spawn -= dt;
            if (g_state->t_spawn < 0)
            {
                g_state->t_spawn = 2;
                g_spawn_enemy(random_point_between_circle(vec2_zero(), 250, 450));
            }
        }

        /** simple ai */
        profiler_scope("simple ai") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_SimpleAI))
                continue;

            entity->heading = heading_to_vec2(entity->position, player->position);
            entity->look_at = heading_to_vec2(entity->position, player->position);
        }

        /** combat ai */
        profiler_scope("combat ai") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_CombatAI))
                continue;

            if (entity->t_attack <= 0)
            {
                entity->t_attack   = entity->attack_rate;
                GameEntity* bullet = g_spawn_bullet(entity->position, entity->look_at, ColliderTypeEnemyAttack, ColorRed500, 24);
            }
        }

        /** lifetime */
        profiler_scope("lifetime") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_Lifetime))
                continue;

            entity->remaining_life -= dt;
            if (entity->remaining_life < 0)
            {
                g_entity_enable_prop(entity, EntityProp_MarkedForDeletion);
            }
        }

        /** rotate towards */
        profiler_scope("rotate towards heading") for_each(entity, g_state->first_entity)
        {
            if (g_entity_has_prop(entity, EntityProp_RotateTowardsHeading))
            {
                entity->rotation = angle_vec2(entity->heading) - 90;
            }
            else if (g_entity_has_prop(entity, EntityProp_RotateTowardsAim))
            {
                entity->rotation = angle_vec2(entity->look_at) - 90;
            }
        }

        /** pull towards */
        profiler_scope("pull towards player") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_PullTowardsPlayer))
                continue;

            entity->force = add_vec2(entity->force, direction_to_vec2(entity->position, player->position, 1));
            if (distsqr_vec2(entity->position, player->position) < 1000)
            {
                g_entity_enable_prop(entity, EntityProp_MarkedForDeletion);
                ps_particle_animation(vec3_xy(entity->position), ANIMATION_GAME_VFX_POWER_UP_PICKUP_EFFECT, random_between_f32(-180, 180));
            }
        }

        /** apply force */
        profiler_scope("force") for_each(entity, g_state->first_entity)
        {
            entity->force    = lerp_vec2(entity->force, vec2_zero(), dt);
            entity->position = add_vec2(entity->position, mul_vec2_f32(entity->force, 10 * dt));
        }

        /** movement */
        profiler_scope("movement") for_each(entity, g_state->first_entity)
        {
            if (g_entity_has_prop(entity, EntityProp_SmoothMovement))
            {
                entity->direction = lerp_vec2(entity->direction, mul_vec2_f32(entity->heading, entity->speed * dt), 4 * dt);
            }
            else
            {
                entity->direction = mul_vec2_f32(entity->heading, entity->speed * dt);
            }
            entity->position = add_vec2(entity->position, entity->direction);
        }

        /** collision */
        profiler_scope("physics")
        {
            uint32        collider_count = 0;
            ColliderInfo* colliders      = arena_push_array(g_state->frame_arena, ColliderInfo, g_state->entity_count);

            profiler_scope("gather colliders") for_each(entity, g_state->first_entity)
            {
                if (!g_entity_has_prop(entity, EntityProp_Collider))
                    continue;

                colliders[collider_count].type     = entity->collider_type;
                colliders[collider_count].position = entity->position;
                colliders[collider_count].radius   = entity->collider_radius;
                colliders[collider_count].entity   = entity;
                collider_count++;
            }

            // SPEED(selim): simple bruteforce implementation
            // TODO(selim): keep previous frame collisions and compare to see if the collision is new or not
            uint32     collision_count = 0;
            Collision* collisions      = arena_push_array(g_state->frame_arena, Collision, collider_count);
            profiler_scope("check collisions") for (uint32 i = 0; i < collider_count; i++)
            {
                for (uint32 j = i + 1; j < collider_count; j++)
                {
                    ColliderInfo a = colliders[i];
                    ColliderInfo b = colliders[j];

                    bool32 can_intersect = bitfield_is_set(g_state->collision_map[a.type], b.type) || bitfield_is_set(g_state->collision_map[b.type], a.type);
                    if (!can_intersect)
                        continue;

                    Intersection i = intersects_circle(circle(a.position, a.radius), circle(b.position, b.radius));
                    if (i.intersects)
                    {
                        collisions[collision_count].a        = a.entity;
                        collisions[collision_count].b        = b.entity;
                        collisions[collision_count].position = lerp_vec2(a.position, b.position, 0.5);
                        collision_count++;
                    }
                }
            }

            profiler_scope("handle collisions") for (uint32 i = 0; i < collision_count; i++)
            {
                Collision* collision = &collisions[i];

                GameEntity* bullet = 0;
                GameEntity* target = 0;

                if (g_entity_has_prop(collision->a, EntityProp_Bullet))
                {
                    bullet = collision->a;
                    target = collision->b;
                }
                if (g_entity_has_prop(collision->b, EntityProp_Bullet))
                {
                    bullet = collision->b;
                    target = collision->a;
                }

                if (bullet)
                {
                    g_entity_enable_prop(bullet, EntityProp_MarkedForDeletion);
                    target->health -= 10;
                    if (target->health <= 0)
                    {
                        g_entity_enable_prop(target, EntityProp_MarkedForDeletion);
                    }
                }

                if (g_entity_has_prop(target, EntityProp_Player))
                {
                    post_processing_add_aberration();
                }
                ps_particle_animation(vec3_xy(collision->position), ANIMATION_GAME_VFX_HIT_EFFECT_PLAYER_BULLET, 0);
            }

            /** editor - physics */
            profiler_scope("editor - physics") if (g_state->editor_active)
            {
                for (uint32 i = 0; i < collider_count; i++)
                {
                    ColliderInfo collider = colliders[i];
                    draw_circle(collider.position, collider.radius, ColorRed400);
                }
            }
        }

        ps_update(dt);
        post_processing_update(g_state->time);
        post_processing_move_camera(vec2_zero(), g_state->time);

        /** render sprites */
        profiler_scope("render sprites") draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_post_processing) for_each(entity, g_state->first_entity)
        {
            if (entity->sprite > 0)
            {
                draw_sprite(entity->position, entity->scale.x, entity->rotation, entity->sprite, vec2_one());
            }
        }
        /** render bullets */
        profiler_scope("render bullets") draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_pixel_perfect) for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_Bullet))
                continue;
            draw_projectile(entity->position, entity->scale.x, entity->color);
        }

        ShaderDataBasic basic_shader = {0};
        r_draw_pass(g_state->pass_pixel_perfect, g_state->pass_post_processing, SORT_LAYER_INDEX_GAME, g_state->material_pass_default, &basic_shader);
        r_draw_pass(g_state->pass_post_processing, g_state->pass_default, SORT_LAYER_INDEX_GAME, g_state->material_post_processing, g_post_processing_state->uniform_data);
        r_render(g_renderer, g_state->time.dt);
        window_update(g_state->window);
    }

    window_destroy(g_state->window);
    logger_flush();
    return 0;
}