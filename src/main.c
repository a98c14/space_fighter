#include "game.h"

#include "game.c"

#define GLYPH_METRICS_CAPACITY 128
#define FREE_GLYPH_FONT_SIZE   16

int
main(void)
{
    game_init();
    const uint32 font_size = 14;

    /* main loop */
    while (!window_should_close(g_state->window))
    {
        arena_reset(g_state->frame_arena);
        if (input_key_pressed(g_state->window, GLFW_KEY_RIGHT_BRACKET))
            break;

        ShaderDataText shader_data    = {0};
        shader_data.color             = d_color_white;
        shader_data.thickness         = d_default_text_thickness;
        shader_data.softness          = d_default_text_softness;
        shader_data.outline_thickness = d_default_text_outline_thickness;

        Rect r = rect_at(vec2(-500, 0), vec2(600, 500), AlignmentLeft);

        draw_debug_rect_b(r);
        r = rect_shrink_f32(r, 10);

        draw_text(string("BottomLeft"), r, ANCHOR_BL_BL, font_size, ColorWhite100);
        draw_text(string("BottomRight"), r, ANCHOR_BR_BR, font_size, ColorWhite100);
        draw_text(string("TopLeft"), r, ANCHOR_TL_TL, font_size, ColorWhite100);
        draw_text(string("TopRight"), r, ANCHOR_TR_TR, font_size, ColorWhite100);

        r_render(g_renderer, g_state->time.dt);
        window_update(g_state->window);
    }

    window_destroy(g_state->window);
    logger_flush();
    return 0;
}