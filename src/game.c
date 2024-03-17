#include "game.h"

internal void
g_init()
{
    /** base */
    logger_init();
    random_init(0x381927afau);

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
    FrameBufferIndex pixel_perfect_frame_buffer   = r_frame_buffer_new(g_renderer, g_renderer->world_width, g_renderer->world_height, GL_NEAREST, ColorInvisible);

    R_PipelineConfiguration* config = r_pipeline_config_new(g_state->frame_arena);
    g_state->pass_default           = r_pipeline_config_add_pass(config, FRAME_BUFFER_INDEX_DEFAULT);
    g_state->pass_post_processing   = r_pipeline_config_add_pass(config, post_processing_frame_buffer);
    g_state->pass_pixel_perfect     = r_pipeline_config_add_pass(config, pixel_perfect_frame_buffer);
    r_pipeline_init(config);

    font_cache_init(persistent_arena);
    draw_context_init(persistent_arena, g_state->frame_arena, g_renderer, g_state->pass_default);
    StringList sprite_texture_paths = {0};
    string_list_push(temp.arena, &sprite_texture_paths, string(ASSET_PATH "\\textures\\game.png"));
    TextureIndex sprite_texture_array = texture_array_from_file(temp.arena, g_renderer, sprite_texture_paths, 1, 1);
    SpriteAtlas* atlas                = sprite_atlas_new(persistent_arena, sprite_texture_array, Animations, Sprites, texture_index_map, array_count(Animations), array_count(Sprites));
    renderer_load_sprite_atlas(g_renderer, atlas);
    draw_context_activate_atlas(atlas);
    post_processing_init(persistent_arena, g_renderer);
    ps_init(persistent_arena, g_state->frame_arena, atlas, 512, g_state->pass_default);

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

    /** physics */
    xassert(ColliderType_COUNT < 64, "There are more collider types than available");
    bitfield_set(&g_state->collision_map[ColliderTypePlayerAttack], ColliderTypeEnemyHitbox);
    bitfield_set(&g_state->collision_map[ColliderTypeEnemyAttack], ColliderTypePlayerHitbox);

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
    return (e->props[prop / (sizeof(EntityProp) * 8)] & 1 << (prop % (sizeof(EntityProp) * 8))) > 0;
}

internal void
g_entity_enable_prop(GameEntity* e, EntityProp prop)
{
    e->props[prop / (sizeof(EntityProp) * 8)] |= 1 << (prop % (sizeof(EntityProp) * 8));
}

/** render */
internal void
draw_projectile(Vec2 pos, float32 radius, Color color)
{
    RenderKey key = render_key_new_default(d_state->ctx->view, d_state->ctx->sort_layer, d_state->ctx->pass, TEXTURE_INDEX_NULL, g_renderer->quad, g_state->material_projectile);

    ShaderDataProjectile shader_data = {0};
    shader_data.color                = color_v4(ColorWhite);
    shader_data.outer_color          = color_v4(color);
    shader_data.slice_ratio          = 1;
    shader_data.fill_ratio           = 1;
    r_draw_single(key, transform_quad_aligned(pos, vec2(radius, radius)), &shader_data);
}

/** utils */
internal GameEntity*
g_spawn_enemy(Vec2 position)
{
    GameEntity* result        = g_entity_alloc();
    result->position          = position;
    result->coin_on_death.max = 10;
    result->speed             = 30;
    result->sprite            = SPRITE_GAME_SHIPS_RANGER;
    result->collider_type     = ColliderTypeEnemyHitbox;
    result->collider_radius   = 26;
    result->attack_rate       = 2;
    result->health            = 80;
    entity_set_color(result, ColorInvisibleWhite);
    entity_set_scale_animation(result, vec2_zero(), vec2_one(), 0.6, EasingTypeEaseOutElastic);
    g_entity_enable_prop(result, EntityProp_RotateTowardsAim);
    g_entity_enable_prop(result, EntityProp_SimpleAI);
    g_entity_enable_prop(result, EntityProp_Collider);
    g_entity_enable_prop(result, EntityProp_CombatAI);
    return result;
}

internal GameEntity*
g_spawn_bullet(Vec2 position, Vec2 direction, ColliderType collider_type, Color color, float32 size, float32 speed, AnimationIndex on_delete_animation)
{
    GameEntity* bullet = g_entity_alloc();
    g_entity_enable_prop(bullet, EntityProp_RotateTowardsHeading);
    g_entity_enable_prop(bullet, EntityProp_Lifetime);
    g_entity_enable_prop(bullet, EntityProp_Bullet);
    g_entity_enable_prop(bullet, EntityProp_Collider);

    bullet->position            = position;
    bullet->heading             = direction;
    bullet->speed               = speed;
    bullet->collider_type       = collider_type;
    bullet->collider_radius     = 10;
    bullet->remaining_life      = 2;
    bullet->damage              = 10;
    bullet->on_delete_animation = on_delete_animation;
    entity_set_color(bullet, color);
    entity_set_scale_animation(bullet, vec2_zero(), vec2(size, size), 0.6, EasingTypeEaseOutElastic);
    return bullet;
}

internal void
entity_add_force(GameEntity* entity, Vec2 force)
{
    entity->force = add_vec2(entity->force, force);
}

internal void
entity_set_scale(GameEntity* entity, Vec2 scale)
{
    entity->scale               = scale;
    entity->anim_scale_end      = scale;
    entity->anim_scale_duration = 1; // set to prevent division by zero during scale animation calculation
}

internal void
entity_set_scale_animation(GameEntity* entity, Vec2 start, Vec2 end, float32 duration, EasingType easing)
{
    entity->scale               = start;
    entity->anim_scale_start    = start;
    entity->anim_scale_end      = end;
    entity->anim_scale_t        = duration;
    entity->anim_scale_duration = duration;
    entity->anim_scale_easing   = easing;
}

internal void
entity_set_color(GameEntity* entity, Color color)
{
    entity->color               = color;
    entity->anim_color_end      = color_v4(color);
    entity->anim_color_duration = 1; // set to prevent division by zero during scale animation calculation
}

internal void
entity_set_color_animation(GameEntity* entity, Color start, Color end, float32 duration, EasingType easing)
{
    entity->color               = start;
    entity->anim_color_start    = color_v4(start);
    entity->anim_color_end      = color_v4(end);
    entity->anim_color_t        = duration;
    entity->anim_color_duration = duration;
    entity->anim_color_easing   = easing;
}