#include "game.h"

#include "game.c"

#define GLYPH_METRICS_CAPACITY 128
#define FREE_GLYPH_FONT_SIZE   16

int
main(void)
{
    g_init();
    const uint32 font_size = 18;

    GameEntity* player = g_entity_alloc();
    player->color      = ColorWhite;
    player->scale      = vec2(24, 24);
    player->speed      = 100;
    g_entity_enable_prop(player, EntityProp_RotateTowardsAim);

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
        }

        float32 dt = g_state->time.dt;

        ShaderDataText shader_data    = {0};
        shader_data.color             = d_color_white;
        shader_data.thickness         = d_default_text_thickness;
        shader_data.softness          = d_default_text_softness;
        shader_data.outline_thickness = d_default_text_outline_thickness;

        // Rect r = rect_at(vec2(-500, 0), vec2(600, 500), AlignmentLeft);
        // draw_debug_rect_b(r);
        // r = rect_shrink_f32(r, 10);

        // draw_text(string("BottomLeft"), r, ANCHOR_BL_BL, font_size, ColorWhite100);
        // draw_text(string("BottomRight"), r, ANCHOR_BR_BR, font_size, ColorWhite100);
        // draw_text(string("TopLeft"), r, ANCHOR_TL_TL, font_size, ColorWhite100);
        // draw_text(string("TopRight"), r, ANCHOR_TR_TR, font_size, ColorWhite100);

        GameEntity* entity;
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

        /** render */
        profiler_scope("render") for_each(entity, g_state->first_entity)
        {
            draw_triangle(entity->position, entity->rotation, entity->color, entity->scale.x);
        }

        r_render(g_renderer, g_state->time.dt);
        window_update(g_state->window);
    }

    window_destroy(g_state->window);
    logger_flush();
    return 0;
}