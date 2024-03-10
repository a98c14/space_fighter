#pragma once

#include <base.h>
#include <engine/draw.h>
#include <engine/profiler.h>
#include <gfx.h>

#define SORT_LAYER_INDEX_SHADOW             3
#define SORT_LAYER_INDEX_BACKGROUND_EFFECTS 4
#define SORT_LAYER_INDEX_GAME               6
#define SORT_LAYER_INDEX_IN_GAME_EFFECTS    9

typedef struct
{
    uint64               render_count;
    SpriteRenderRequest* requests;
    int32                layer_counts[SORTING_LAYER_CAPACITY];
} PS_PassRenderGroup;

typedef uint64 ParticleIndex;

typedef enum
{
    PS_ParticleKindDead,
    PS_ParticleKindQuad,
    PS_ParticleKindSprite,
    PS_ParticleKindAnimation,
    PS_ParticleKindText,
} PS_ParticleKind;

typedef enum
{
    PS_EffectFlagsNone           = 0 << 0,
    PS_EffectFlagsScaleAnimation = 1 << 0,
    PS_EffectFlagsColorAnimation = 1 << 1,
    PS_EffectFlagsDirection      = 1 << 2,
} PS_EffectFlags;

typedef struct
{
    float32    target_scale;
    EasingType easing;
} PS_EffectScaleAnimation;

typedef struct
{
    Vec3       direction;
    EasingType easing;
} PS_EffectDirection;

typedef struct
{
    Vec4       color;
    Vec4       outline_color;
    EasingType easing;
} PS_EffectColorAnimation;

typedef struct
{
    float32 delay_t;
    float32 remaining_t;
    float32 total_t;

    PS_EffectFlags effects;
    Vec3           starting_position;
    float32        starting_scale;
    SortLayerIndex sort_layer;
    PassIndex      pass;
    Vec4           starting_color;

    union
    {
        SpriteIndex    sprite;
        AnimationIndex animation;
        String4        text;
    };
} PS_Particle;

typedef struct
{
    SpriteIndex sprite;
    float32     rotation;
    Color       color;
    Vec2        position;
    Vec2        scale;
} PS_ParticleRenderInfo;

typedef struct
{
    Arena* temp_arena;

    /** settings */
    float32      animation_update_rate;
    SpriteAtlas* atlas;
    PassIndex    default_pass;

    /** ring buffer */
    uint64 active_particle_count_per_pass[PASS_CAPACITY];

    uint64 particle_capacity;
    uint64 particle_start;
    uint64 particle_end;

    PS_ParticleKind* particle_kind_buffer;
    PS_Particle*     particle_buffer;

    PS_EffectScaleAnimation* effect_scale_buffer;
    PS_EffectDirection*      effect_direction_buffer;
    PS_EffectColorAnimation* effect_color_buffer;
} PS_State;
global PS_State* ps_state;

internal void ps_init(Arena* arena, Arena* frame_arena, SpriteAtlas* atlas, uint64 capacity, PassIndex default_pass);
internal void ps_update(float32 dt);
internal void ps_render_group_add(PS_PassRenderGroup* pass_groups, PassIndex pass, SortLayerIndex sort_layer, SpriteRenderRequest request);

/** spawn functions */
internal ParticleIndex ps_particle_new_internal(PS_ParticleKind kind, Vec3 position, float32 duration, PassIndex pass, SortLayerIndex sort_layer);
internal ParticleIndex ps_particle_animation_delayed(Vec3 position, AnimationIndex animation, float32 delay);
internal ParticleIndex ps_particle_animation(Vec3 position, AnimationIndex animation);
internal ParticleIndex ps_particle_text(Vec3 position, String str, float32 duration, Color color, float32 size);
internal ParticleIndex ps_particle_sprite(Vec3 position, SpriteIndex sprite, float32 duration);
internal ParticleIndex ps_particle_pixel_perfect_sprite(Vec3 position, SpriteIndex sprite, float32 duration);

/** effect functions */
internal void ps_set_scale_animation(ParticleIndex particle, EasingType easing, float32 size);
internal void ps_set_color_animation(ParticleIndex particle, EasingType easing, Color color);
internal void ps_set_direction(ParticleIndex particle, EasingType easing, Vec3 v);
internal void ps_set_delay(ParticleIndex particle, float32 delay);
internal void ps_set_layer_and_pass(ParticleIndex particle, PassIndex pass, SortLayerIndex layer);