#include "game.h"

#include "game.c"

#define GLYPH_METRICS_CAPACITY 128
#define FREE_GLYPH_FONT_SIZE   16

int
main(void)
{
    g_init();
    const uint32 font_size = 18;

    GameEntity* player  = g_entity_alloc();
    player->color       = ColorWhite;
    player->scale       = vec2(1, 1);
    player->speed       = 100;
    player->sprite      = SPRITE_GAME_SHIPS_RED_BEATLE;
    player->attack_rate = 0.3;
    g_entity_enable_prop(player, EntityProp_RotateTowardsAim);

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
                player->t_attack   = player->attack_rate;
                GameEntity* bullet = g_entity_alloc();
                g_entity_enable_prop(bullet, EntityProp_RotateTowardsHeading);
                g_entity_enable_prop(bullet, EntityProp_Lifetime);
                g_entity_enable_prop(bullet, EntityProp_Bullet);
                g_entity_enable_prop(bullet, EntityProp_Collider);
                bullet->position        = player->position;
                bullet->heading         = player->look_at;
                bullet->scale           = vec2(24, 24);
                bullet->color           = ColorWhite;
                bullet->speed           = 200;
                bullet->collider_type   = ColliderTypePlayerAttack;
                bullet->collider_radius = 10;
                bullet->remaining_life  = 2;
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
                    coin->animation  = ANIMATION_GAME_COLLECTABLES_EXPERIENCE_ORB;
                    coin->position   = random_point_in_circle(entity->position, 30);
                    coin->scale      = vec2(1, 1);
                    coin->color      = ColorWhite;
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
                g_spawn_enemy(vec2(100, 100));
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

        /** movement */
        profiler_scope("movement") for_each(entity, g_state->first_entity)
        {
            Vec2 direction   = mul_vec2_f32(entity->heading, entity->speed * dt);
            entity->position = add_vec2(entity->position, direction);
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

                    // TODO(selim): create a lookup array for collision types
                    bool32 can_intersect = (a.type == ColliderTypePlayerAttack && b.type == ColliderTypeEnemyHitbox) || (a.type == ColliderTypeEnemyHitbox && b.type == ColliderTypePlayerAttack);
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

                ps_particle_animation(vec3_xy(collision->position), ANIMATION_GAME_VFX_HIT_EFFECT_PLAYER_BULLET);
            }

            /** editor - physics */
            profiler_scope("editor - physics")
            {
                for (uint32 i = 0; i < collider_count; i++)
                {
                    ColliderInfo collider = colliders[i];
                    draw_circle(collider.position, collider.radius, ColorRed400);
                }
            }
        }

        ps_update(dt);

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
            draw_projectile(entity->position, entity->scale.x);
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