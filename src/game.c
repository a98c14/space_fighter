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
    ArenaTemp temp = scratch_begin(0, 0);

    /** renderer */
    RendererConfiguration* r_config = r_config_new(g_state->frame_arena);
    r_config_set_screen_size(r_config, WINDOW_WIDTH, WINDOW_HEIGHT);
    r_config_set_world_size(r_config, 0, WORLD_HEIGHT);
    r_config_set_clear_color(r_config, ColorSlate900);
    renderer_init(persistent_arena, r_config);

    /** pipelines */
    const float32    pp_res_multiplier            = 3.0f;
    FrameBufferIndex post_processing_frame_buffer = r_frame_buffer_new(g_renderer, g_renderer->world_width * pp_res_multiplier, g_renderer->world_height * pp_res_multiplier, GL_LINEAR, ColorInvisible);
    FrameBufferIndex pixel_perfect_frame_buffer   = r_frame_buffer_new(g_renderer, g_renderer->world_width, g_renderer->world_height, GL_LINEAR, ColorInvisible);

    R_PipelineConfiguration* config = r_pipeline_config_new(g_state->frame_arena);
    g_state->pass_default           = r_pipeline_config_add_pass(config, FRAME_BUFFER_INDEX_DEFAULT);
    g_state->pass_post_processing   = r_pipeline_config_add_pass(config, post_processing_frame_buffer);
    g_state->pass_pixel_perfect     = r_pipeline_config_add_pass(config, pixel_perfect_frame_buffer);
    r_pipeline_init(config);

    font_cache_init(persistent_arena);
    draw_context_init(persistent_arena, g_state->frame_arena, g_renderer, g_state->pass_default);
    StringList sprite_texture_paths = {0};
    string_list_push(temp.arena, &sprite_texture_paths, string(ASSET_PATH "\\textures\\game.png"));
    TextureIndex sprite_texture_array = texture_array_from_file(temp.arena, g_renderer, sprite_texture_paths, 0, 1);
    SpriteAtlas* atlas                = sprite_atlas_new(persistent_arena, sprite_texture_array, Animations, Sprites, texture_index_map, array_count(Animations), array_count(Sprites));
    renderer_load_sprite_atlas(g_renderer, atlas);
    draw_context_activate_atlas(atlas);
    post_processing_init(persistent_arena, g_renderer);

    /** assets */
    {
        g_state->material_projectile = material_new(
            g_renderer,
            file_read_all_as_string(temp.arena, string(SHADER_PATH "\\enemy_projectile.vert")),
            file_read_all_as_string(temp.arena, string(SHADER_PATH "\\enemy_projectile.frag")),
            sizeof(ShaderDataProjectile),
            true);

        g_state->material_post_processing = material_new(
            g_renderer,
            file_read_all_as_string(temp.arena, string(SHADER_PATH "\\post_processing.vert")),
            file_read_all_as_string(temp.arena, string(SHADER_PATH "\\post_processing.frag")),
            sizeof(ShaderDataPostProcessing),
            false);

        g_state->material_pass_default = material_new(
            g_renderer,
            file_read_all_as_string(temp.arena, string(SHADER_PATH "\\frame_buffer_pixel_perfect.vert")),
            file_read_all_as_string(temp.arena, string(SHADER_PATH "\\frame_buffer_pixel_perfect.frag")),
            sizeof(ShaderDataBasic),
            false);
    }

    FontFaceIndex ibx_mono = font_load(string("ibx_mono"), string(ASSET_PATH "\\IBMPlexMono-Bold.ttf"), GlyphAtlasTypeFreeType);
    draw_activate_font(ibx_mono);
    scratch_end(temp);
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

/** render */
internal void
draw_projectile(Vec2 pos, float32 radius)
{
    RenderKey key = render_key_new(d_state->ctx->view, d_state->ctx->sort_layer, d_state->ctx->pass, TEXTURE_INDEX_NULL, g_renderer->quad, g_state->material_projectile);

    ShaderDataProjectile shader_data = {0};
    shader_data.color                = color_v4(ColorWhite);
    shader_data.outer_color          = color_v4(ColorRed500);
    shader_data.slice_ratio          = 1;
    shader_data.fill_ratio           = 1;
    r_draw_single(key, transform_quad_aligned(pos, vec2(radius, radius)), &shader_data);
}
