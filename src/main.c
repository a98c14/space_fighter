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

    GameEntity* player               = g_entity_alloc();
    player->speed                    = 125;
    player->sprite                   = SPRITE_GAME_SHIPS_RED_BEATLE;
    player->attack_rate              = 0.1;
    player->collider_type            = ColliderTypePlayerHitbox;
    player->health                   = 5;
    player->invulnerability_duration = 1;
    player->collider_radius          = 16;
    player->projectile_count         = 1;
    player->bullet_spawn_offset      = vec2(0, 9);
    player->look_at                  = vec2(1, 0);
    entity_set_scale(player, vec2_one());
    entity_set_color(player, ColorInvisibleWhite);
    g_entity_enable_prop(player, EntityProp_Player);
    g_entity_enable_prop(player, EntityProp_SmoothMovement);
    g_entity_enable_prop(player, EntityProp_RotateTowardsAim);
    g_entity_enable_prop(player, EntityProp_Collider);

    g_state->t_spawn = 2;

    Trail* right_wing_trail = trail_new(g_state->persistent_arena, 16);
    Trail* left_wing_trail  = trail_new(g_state->persistent_arena, 16);
    trail_set_color(left_wing_trail, ColorWhite, ColorWhite);
    trail_set_width(left_wing_trail, 0.8, 0);
    trail_set_color(right_wing_trail, ColorWhite, ColorWhite);
    trail_set_width(right_wing_trail, 0.8, 0);

    /** setup background objects */
    {
        g_state->background_objects = arena_push_array_zero(g_state->persistent_arena, BackgroundObject, 256);
        const SpriteIndex stars[]   = {
            SPRITE_GAME_CELESTIAL_OBJECTS_STAR_0,
            SPRITE_GAME_CELESTIAL_OBJECTS_STAR_1,
            SPRITE_GAME_CELESTIAL_OBJECTS_STAR_2,
            SPRITE_GAME_CELESTIAL_OBJECTS_STAR_3,
            SPRITE_GAME_CELESTIAL_OBJECTS_STAR_4,
        };

        for (uint32 i = 0; i < 20; i++)
        {
            g_state->background_objects[g_state->background_object_count++] = (BackgroundObject){.parallax_scale = 1.03, .position = vec2(random_between_f32(-400, 400), random_between_f32(-150, 150)), .rotation = random_between_f32(-180, 180), .sprite = stars[random_between_i32(0, array_count(stars))]};
        }

        g_state->background_objects[g_state->background_object_count++] = (BackgroundObject){.parallax_scale = 1.045, .position = vec2(100, -20), .sprite = SPRITE_GAME_CELESTIAL_OBJECTS_NEBULA_0};
        g_state->background_objects[g_state->background_object_count++] = (BackgroundObject){.parallax_scale = 1.045, .position = vec2(100, -100), .sprite = SPRITE_GAME_CELESTIAL_OBJECTS_NEBULA_1};
        g_state->background_objects[g_state->background_object_count++] = (BackgroundObject){.parallax_scale = 1.06, .position = vec2(150, -60), .sprite = SPRITE_GAME_CELESTIAL_OBJECTS_PLANET_0};
        g_state->background_objects[g_state->background_object_count++] = (BackgroundObject){.parallax_scale = 1.06, .position = vec2(100, 60), .sprite = SPRITE_GAME_CELESTIAL_OBJECTS_PLANET_1};
        g_state->background_objects[g_state->background_object_count++] = (BackgroundObject){.parallax_scale = 1.1, .position = vec2(50, -20), .sprite = SPRITE_GAME_CELESTIAL_OBJECTS_PLANET_2};
    }
    glClearColor(0, 0, 0, 1);
    OE_AudioHandle gun_sound = oe_audio_handle_from_path(string(ASSET_PATH "\\audio\\gun.mp3"));

    /** main loop */
    while (!window_should_close(g_state->window))
    {
        profiler_begin("frame");
        arena_reset(g_state->frame_arena);
        draw_text(string(VERSION_NUMBER), rect_shrink_f32(screen_rect(), 8), ANCHOR_BR_BR, 7, ColorWhite);
        g_state->time = engine_get_time(g_state->time);
        if (input_key_pressed_raw(g_state->window, GLFW_KEY_RIGHT_BRACKET))
            break;

        float32 dt = g_state->time.dt;
        if (g_state_enabled(GameStateFlagPaused))
        {
            post_processing_set_saturation(0);
            dt = 0;
        }
        else
        {
            post_processing_set_saturation(1);
        }

        if (g_state->player_level < MAX_LEVEL_COUNT && g_state->player_experience > g_state->experience_requirement[g_state->player_level + 1])
        {
            g_state_enable(GameStateFlagLevelUp);
        }

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
                    coin->force               = random_direction(200);
                    coin->position            = entity->position;
                    entity_set_color(player, ColorInvisibleWhite);
                    entity_set_scale_animation(coin, vec2_zero(), vec2_one(), 0.6, EasingTypeEaseOutElastic);
                }
            }

            g_entity_free(entity);
        }

        game_ui_update();
        input_manager_update(g_state->time);

        /** editor */
        profiler_scope("editor") if (g_state_enabled(GameStateFlagEditor))
        {
            ArenaTemp     temp        = scratch_begin(0, 0);
            const float32 line_height = 8;
            const float32 text_size   = 7;
            Rect          screen      = screen_rect();
            Rect          editor      = rect_anchor(rect_from_wh(300, 80), rect_shrink_f32(screen, 16), ANCHOR_TL_TL);
            draw_rect(editor, ColorWhite100A);
            editor = rect_shrink_f32(editor, 8);
            for (uint32 i = 1; i < g_input_manager->key_count; i++)
            {
                InputState state        = g_input_manager->key_states[i];
                Rect       line         = rect_cut_top(&editor, line_height);
                char*      pressed_text = (state.key_state & InputKeyStatePressed) > 0 ? "pressed" : "released";
                char*      new_text     = (state.key_state & InputKeyStateNew) > 0 ? "new" : "old";

                draw_text(g_input_manager->key_names[i], line, ANCHOR_TL_TL, text_size, ColorBlack);
                rect_cut_left(&line, 50);
                draw_text(string_pushf(temp.arena, "%s (%s)", pressed_text, new_text), line, ANCHOR_TL_TL, text_size, ColorBlack);
                rect_cut_left(&line, 64);
                draw_text(string_pushf(temp.arena, "press: %.2f, release: %.2f", state.t_press, state.t_release), line, ANCHOR_TL_TL, text_size, ColorBlack);
            }
            scratch_end(temp);
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

        /** apply force */
        profiler_scope("force") for_each(entity, g_state->first_entity)
        {
            entity->force    = add_vec2(entity->force, mul_vec2_f32(entity->force, -dt * 0.5));
            entity->position = add_vec2(entity->position, mul_vec2_f32(entity->force, dt));
        }

        /** gather input */
        bool32 gas = false;
        {
            if (input_key_pressed_raw(g_state->window, GLFW_KEY_SPACE))
            {
                gas = true;
            }

            float32 angular_change = dt * 8;
            float32 angular_speed  = 400;
            if (input_key_pressed_raw(g_state->window, GLFW_KEY_W))
            {
                player->force = add_vec2(player->force, mul_vec2_f32(player->look_at, player->speed * dt));
                angular_speed -= 200;
            }
            if (input_key_pressed_raw(g_state->window, GLFW_KEY_S))
            {
                player->force = add_vec2(player->force, mul_vec2_f32(player->look_at, -player->speed * dt * 0.6));
            }

            if (input_key_pressed_raw(g_state->window, GLFW_KEY_A))
            {
                player->angular_speed = lerp_f32(player->angular_speed, angular_speed, angular_change);
            }
            if (input_key_pressed_raw(g_state->window, GLFW_KEY_D))
            {
                player->angular_speed = lerp_f32(player->angular_speed, -angular_speed, angular_change);
            }
            else
            {
                player->angular_speed = lerp_f32(player->angular_speed, 0, angular_change);
            }
            player->angular_speed *= gas ? 0.7 : 1;

            g_state->input_mouse = input_mouse_get(g_state->window, g_renderer->camera, g_state->input_mouse);
        }

        /** apply input */
        if (!g_state_enabled(GameStateFlagPaused))
        {
            if (input_action_pressed(GameKeyEditor))
            {
                g_state_toggle(GameStateFlagEditor);
            }

            if (input_action_pressed(GameKeyPause))
            {
                g_state_toggle(GameStateFlagPaused);
                g_state_toggle(GameStateFlagLevelUp);
            }

            // player->look_at = heading_to_vec2(player->position, g_state->input_mouse.world);
            player->look_at = rotate_vec2(player->look_at, player->angular_speed * dt);
            if (gas > 0)
            {
                player->force = add_vec2(player->force, mul_vec2_f32(player->look_at, 300 * dt));
            }

            if (gas == 0)
            {
                player->force = lerp_vec2(player->force, clamp_vec2_length(0, player->force, 200), dt);
            }

            if (input_mouse_held(g_state->input_mouse, MouseButtonStateLeft) && player->t_attack <= 0)
            {
                // TODO(selim):  move bullet firing code to separate place
                player->t_attack        = player->attack_rate;
                Vec2    bullet_position = add_vec2(player->position, rotate_vec2(player->bullet_spawn_offset, player->rotation));
                float32 starting_angle  = angle_vec2(player->look_at) - (uint32)(player->projectile_count / 2) * 15;
                oe_audio_play(gun_sound);
                for (uint32 i = 0; i < player->projectile_count; i++)
                {
                    float32 angle     = starting_angle + i * 15;
                    Vec2    direction = rotate_vec2(vec2(1, 0), angle);
                    g_spawn_bullet(bullet_position, direction, ColliderTypePlayerAttack, ColorYellow500, 12, 240, ANIMATION_GAME_VFX_HIT_EFFECT_PLAYER_BULLET);
                    ParticleIndex p = ps_particle_animation(vec3_xy(bullet_position), ANIMATION_GAME_VFX_MUZZLE_FLASH_1, angle);
                    player->force   = add_vec2(player->force, mul_vec2_f32(player->look_at, -2));
                }
                post_processing_add_shake(2);
            }
        }

        /** enemy spawner */
        profiler_scope("enemy spawner")
        {
            g_state->t_spawn -= dt;
            if (g_state->t_spawn < 0)
            {
                g_state->t_spawn = 2;
                g_spawn_enemy(random_point_between_circle(add_vec2(player->position, mul_vec2_f32(norm_vec2_safe(player->force), -200)), 30, 100));
            }
        }

        /** destroy out of range enemies */
        const float32 destroy_range_sqr = powf(300, 2);
        profiler_scope("destroy out of range enemies") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_DestroyWhenAwayFromPlayer))
                continue;
            float32 dsqr = distsqr_vec2(player->position, entity->position);
            if (dsqr > destroy_range_sqr)
            {
                entity->t_out_of_range += dt;
                if (entity->t_out_of_range > 1)
                {
                    g_entity_enable_prop(entity, EntityProp_MarkedForDeletion);
                }
            }
            else
            {
                entity->t_out_of_range = 0;
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
            entity->t_attack          = max(0, entity->t_attack - dt);
            entity->t_invulnerability = max(0, entity->t_invulnerability - dt);
        }

        /** simple ai */
        profiler_scope("simple ai") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_SimpleAI))
                continue;

            // entity->heading = heading_to_vec2(entity->position, player->position);
            entity->look_at = heading_to_vec2(entity->position, player->position);
            entity->force   = add_vec2(entity->force, mul_vec2_f32(entity->look_at, entity->speed * dt));
            entity->force   = clamp_vec2_length(0, entity->force, entity->speed);
        }

        /** combat ai */
        profiler_scope("combat ai") for_each(entity, g_state->first_entity)
        {
            if (!g_entity_has_prop(entity, EntityProp_CombatAI))
                continue;

            if (entity->t_attack <= 0)
            {
                entity->t_attack   = entity->attack_rate;
                GameEntity* bullet = g_spawn_bullet(entity->position, entity->look_at, ColliderTypeEnemyAttack, ColorRed500, 24, 300, ANIMATION_GAME_VFX_HIT_EFFECT_ENEMY_BULLET);
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
            entity->force = add_vec2(entity->force, direction_to_vec2(entity->position, player->position, dt * 1000));
            if (distsqr_vec2(entity->position, player->position) < 1000)
            {
                g_state->player_experience++;
                g_entity_enable_prop(entity, EntityProp_MarkedForDeletion);
            }
        }

        /** property animations */
        profiler_scope("property animations") for_each(entity, g_state->first_entity)
        {
            // scale
            {
                float32 t            = 1 - (entity->anim_scale_t / entity->anim_scale_duration);
                entity->scale        = lerp_vec2(entity->anim_scale_start, entity->anim_scale_end, ease_dynamic(t, entity->anim_scale_easing));
                entity->anim_scale_t = max(entity->anim_scale_t - dt, 0);
            }

            // color
            {
                float32 t            = 1 - (entity->anim_color_t / entity->anim_color_duration);
                entity->color        = vec4_to_color(lerp_vec4(entity->anim_color_start, entity->anim_color_end, ease_dynamic(t, entity->anim_color_easing)));
                entity->anim_color_t = max(entity->anim_color_t - dt, 0);
            }
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

                if (bullet && target->t_invulnerability <= 0)
                {
                    g_entity_enable_prop(bullet, EntityProp_MarkedForDeletion);
                    entity_set_color_animation(target, ColorWhite, ColorInvisibleWhite, 0.3, EasingTypeEaseOutCubic);
                    entity_set_scale_animation(target, vec2(1.3, 1.3), vec2_one(), 0.3, EasingTypeEaseOutElastic);
                    entity_add_force(target, mul_vec2_f32(bullet->heading, 30));
                    target->health -= 1;
                    target->t_invulnerability = target->invulnerability_duration;
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
            profiler_scope("editor - physics") if (g_state_enabled(GameStateFlagEditor))
            {
                draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_default) for (uint32 i = 0; i < collider_count; i++)
                {
                    ColliderInfo collider = colliders[i];
                    draw_circle(collider.position, collider.radius, ColorRed400);
                }
            }
        }

        /** draw trail */
        profiler_scope("draw trail") draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_pixel_perfect)
        {
            if (gas)
            {
                Vec2 normal = vec2(-player->look_at.y, player->look_at.x);
                trail_push_position(left_wing_trail, add_vec2(player->position, mul_vec2_f32(normal, 12)));
                trail_push_position(right_wing_trail, add_vec2(player->position, mul_vec2_f32(normal, -12)));

                trail_draw(left_wing_trail);
                trail_draw(right_wing_trail);
            }
            else
            {
                trail_reset(left_wing_trail);
                trail_reset(right_wing_trail);
            }
        }

        ps_update(dt);
        post_processing_update(g_state->time);
        g_state->camera_position = lerp_vec2(g_state->camera_position, player->position, 8 * dt);
        post_processing_move_camera(g_state->camera_position, g_state->time);

        /** background */
        profiler_scope("background") draw_scope(SORT_LAYER_INDEX_GROUND, ViewTypeWorld, g_state->pass_post_processing)
        {
            for (uint32 i = 0; i < g_state->background_object_count; i++)
            {
                BackgroundObject obj = g_state->background_objects[i];
                draw_sprite(add_vec2(div_vec2_f32(g_state->camera_position, obj.parallax_scale), obj.position), 1, obj.rotation, obj.sprite, vec2_one());
            }
        }

        /** render sprites */
        profiler_scope("render sprites") draw_scope(SORT_LAYER_INDEX_GAME, ViewTypeWorld, g_state->pass_pixel_perfect) for_each(entity, g_state->first_entity)
        {
            if (entity->sprite > 0)
            {
                draw_sprite_colored(entity->position, entity->scale.x, entity->rotation, entity->sprite, vec2_one(), entity->color, entity->t_invulnerability > 0 ? .8 : 1);
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
        profiler_end();
    }

    window_destroy(g_state->window);
    oe_audio_shutdown();
    logger_flush();
    return 0;
}