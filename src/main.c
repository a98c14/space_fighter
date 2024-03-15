#ifndef VERSION_NUMBER
#define VERSION_NUMBER "version-0.0.0"
#endif
#include "game.h"

#include "game.c"

#define GLYPH_METRICS_CAPACITY 128
#define FREE_GLYPH_FONT_SIZE   16

int
main(void)
{
    ThreadContext tctx;
    tctx_init_and_equip(&tctx);
    g_init();
    const uint32 font_size = 18;

    GameEntity* player          = g_entity_alloc();
    player->color               = ColorWhite;
    player->scale               = vec2(1, 1);
    player->speed               = 120;
    player->sprite              = SPRITE_GAME_SHIPS_RED_BEATLE;
    player->attack_rate         = 0.1;
    player->collider_type       = ColliderTypePlayerHitbox;
    player->health              = 100;
    player->collider_radius     = 20;
    player->projectile_count    = 1;
    player->bullet_spawn_offset = vec2(0, 18);
    player->look_at             = vec2(1, 0);
    g_entity_enable_prop(player, EntityProp_Player);
    g_entity_enable_prop(player, EntityProp_SmoothMovement);
    g_entity_enable_prop(player, EntityProp_RotateTowardsAim);
    g_entity_enable_prop(player, EntityProp_Collider);

    bool32 is_paused = false;

    int32 trail_count             = 0;
    int32 test_trail_index        = 0;
    Vec2  test_trail_buffer[16]   = {0};
    Vec2  test_trail_buffer_2[16] = {0};

    /* main loop */
    while (!window_should_close(g_state->window))
    {
        arena_reset(g_state->frame_arena);
        draw_text(string(VERSION_NUMBER), rect_shrink_f32(screen_rect(), 8), ANCHOR_BR_BR, 12, ColorWhite);
        g_state->time = engine_get_time(g_state->time);
        if (input_key_pressed(g_state->window, GLFW_KEY_RIGHT_BRACKET))
            break;

        float32 dt = g_state->time.dt;
        if (is_paused)
            dt = 0;

        /** gather input */
        bool32 gas = false;
        {
            if (input_key_pressed(g_state->window, GLFW_KEY_SPACE))
            {
                gas = true;
            }

            float32 angular_change = dt * 8;
            if (input_key_pressed(g_state->window, GLFW_KEY_A))
            {
                player->angular_speed = lerp_f32(player->angular_speed, 200, angular_change);
            }
            else if (input_key_pressed(g_state->window, GLFW_KEY_D))
            {
                player->angular_speed = lerp_f32(player->angular_speed, -200, angular_change);
            }
            else
            {
                player->angular_speed = lerp_f32(player->angular_speed, 0, angular_change);
            }
            player->angular_speed *= gas ? 0.8 : 1;

            g_state->input_mouse = input_mouse_get(g_state->window, g_renderer->camera, g_state->input_mouse);
        }

        /** apply input */
        {
            // player->look_at = heading_to_vec2(player->position, g_state->input_mouse.world);
            player->look_at = rotate_vec2(player->look_at, player->angular_speed * dt);
            if (gas > 0)
                player->force = add_vec2(player->force, mul_vec2_f32(player->look_at, 10));

            player->force = clamp_vec2_length(0, player->force, 400);

            if (input_mouse_held(g_state->input_mouse, MouseButtonStateLeft) && player->t_attack <= 0)
            {
                // TODO(selim):  move bullet firing code to separate place
                player->t_attack        = player->attack_rate;
                Vec2    bullet_position = add_vec2(player->position, rotate_vec2(player->bullet_spawn_offset, player->rotation));
                float32 starting_angle  = angle_vec2(player->look_at) - (uint32)(player->projectile_count / 2) * 15;
                for (uint32 i = 0; i < player->projectile_count; i++)
                {
                    float32 angle     = starting_angle + i * 15;
                    Vec2    direction = rotate_vec2(vec2(1, 0), angle);
                    g_spawn_bullet(bullet_position, direction, ColliderTypePlayerAttack, ColorYellow500, 12, 500, ANIMATION_GAME_VFX_HIT_EFFECT_PLAYER_BULLET);
                    ParticleIndex p = ps_particle_animation(vec3_xy(bullet_position), ANIMATION_GAME_VFX_MUZZLE_FLASH_1, angle);
                }
                post_processing_add_shake(2);
            }
        }

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

            if (entity->on_delete_animation > 0)
            {
                ps_particle_animation(vec3_xy(entity->position), entity->on_delete_animation, random_between_f32(-180, 180));
            }

            bool32 drops_coin = entity->coin_on_death.min > 0 || entity->coin_on_death.max > 0;
            if (drops_coin)
            {
                uint32 coin_count = random_between_i32(entity->coin_on_death.min, entity->coin_on_death.max);
                for (uint32 i = 0; i < coin_count; i++)
                {
                    GameEntity* coin = g_entity_alloc();
                    g_entity_enable_prop(coin, EntityProp_PullTowardsPlayer);
                    coin->animation           = ANIMATION_GAME_COLLECTABLES_EXPERIENCE_ORB;
                    coin->on_delete_animation = ANIMATION_GAME_VFX_EXPERIENCE;
                    coin->force               = random_direction(20);
                    coin->position            = entity->position;
                    coin->scale               = vec2(1, 1);
                    coin->color               = ColorWhite;
                }
            }

            g_entity_free(entity);
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
                GameEntity* bullet = g_spawn_bullet(entity->position, entity->look_at, ColliderTypeEnemyAttack, ColorRed500, 24, 100, ANIMATION_GAME_VFX_HIT_EFFECT_ENEMY_BULLET);
            }
        }

        /** lifetime */
        profiler_scope("lifetime") for_each(entity, g_state->first_entity)
        {
            entity->t_alive += dt;
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

            entity->force = lerp_vec2(entity->force, vec2_zero(), dt);
            entity->force = add_vec2(entity->force, direction_to_vec2(entity->position, player->position, entity->t_alive));
            if (distsqr_vec2(entity->position, player->position) < 1000)
            {
                g_entity_enable_prop(entity, EntityProp_MarkedForDeletion);
            }
        }

        /** apply force */
        profiler_scope("force") for_each(entity, g_state->first_entity)
        {
            entity->position = add_vec2(entity->position, mul_vec2_f32(entity->force, dt));
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

        trail_count                                  = min(trail_count + 1, array_count(test_trail_buffer));
        Vec2 normal                                  = vec2(-player->look_at.y, player->look_at.x);
        test_trail_buffer[(test_trail_index) % 16]   = add_vec2(player->position, mul_vec2_f32(normal, 12));
        test_trail_buffer_2[(test_trail_index) % 16] = add_vec2(player->position, mul_vec2_f32(normal, -12));
        test_trail_index                             = (test_trail_index + 1) % 16;

        /** draw trail */
        profiler_scope("draw trail")
        {
            draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_pixel_perfect)
            {
                VertexBuffer* trail = draw_util_generate_trail_vertices_fast(g_state->frame_arena, test_trail_buffer, trail_count, test_trail_index, 0.2, 0.8);
                draw_trail(trail->v, trail->count, ColorWhite, ColorInvisibleWhite);
                VertexBuffer* trail2 = draw_util_generate_trail_vertices_fast(g_state->frame_arena, test_trail_buffer_2, trail_count, test_trail_index, 0.2, 0.8);
                draw_trail(trail2->v, trail2->count, ColorWhite, ColorInvisibleWhite);
            }
        }

        ps_update(dt);
        post_processing_update(g_state->time);
        post_processing_move_camera(player->position, g_state->time);

        /** render sprites */
        profiler_scope("render sprites") draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_pixel_perfect) for_each(entity, g_state->first_entity)
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