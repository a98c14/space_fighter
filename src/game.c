#include "game.h"

internal void
g_init()
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
    profiler_init(persistent_arena);

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

internal GameEntity*
g_entity_alloc()
{
    GameEntity* result = g_state->free_entities;
    stack_pop(g_state->free_entities);
    if (!result)
    {
        result = arena_push_struct_zero(g_state->persistent_arena, GameEntity);
    }

    result->gen++;
    dll_push_back(g_state->first_entity, g_state->last_entity, result);
    g_state->entity_count++;
    return result;
}

internal void
g_entity_free(GameEntity* e)
{
    dll_remove(g_state->first_entity, g_state->last_entity, e);
    // zero everything but the generation
    uint64 generation = e->gen;
    memory_zero_struct(e);
    e->gen = generation;
    stack_push(g_state->free_entities, e);
    g_state->entity_count--;
}

internal bool32
g_entity_has_prop(GameEntity* e, EntityProp prop)
{
    return (e->props[prop / sizeof(EntityProp)] & 1 << (prop % sizeof(EntityProp))) > 0;
}

internal void
g_entity_enable_prop(GameEntity* e, EntityProp prop)
{
    e->props[prop / sizeof(EntityProp)] |= 1 << (prop % sizeof(EntityProp));
}