#include "game.h"

internal void
game_init()
{
    /** base */
    ThreadContext tctx;
    tctx_init_and_equip(&tctx);
    logger_init();

    Arena* persistent_arena   = make_arena_reserve(mb(128));
    g_state                   = arena_push_struct_zero(persistent_arena, GameState);
    g_state->persistent_arena = persistent_arena;
    g_state->frame_arena      = make_arena_reserve(mb(128));
    g_state->window           = window_create(persistent_arena, WINDOW_WIDTH, WINDOW_HEIGHT, "Scratch Window", NULL);
    g_state->time             = engine_time_new();

    /** renderer */
    RendererConfiguration* r_config = r_config_new(g_state->frame_arena);
    r_config_set_screen_size(r_config, WINDOW_WIDTH, WINDOW_HEIGHT);
    r_config_set_world_size(r_config, 0, WINDOW_HEIGHT);
    r_config_set_clear_color(r_config, ColorSlate900);
    renderer_init(persistent_arena, r_config);

    /** pipelines */
    R_PipelineConfiguration* config       = r_pipeline_config_new(g_state->frame_arena);
    PassIndex                pass_default = r_pipeline_config_add_pass(config, FRAME_BUFFER_INDEX_DEFAULT);
    r_pipeline_init(config);
    font_cache_init(persistent_arena);
    draw_context_init(persistent_arena, g_state->frame_arena, g_renderer, pass_default);

    /** assets */
    FontFaceIndex ibx_mono = font_load(string("ibx_mono"), string(ASSET_PATH "\\IBMPlexMono-Bold.ttf"), GlyphAtlasTypeFreeType);
    draw_activate_font(ibx_mono);
}