#pragma once
// #include "ft2build.h"
// #include FT_FREETYPE_H
#include <corecrt_math.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "user32.lib")

#define SHADER_PATH "..\\shaders"
#define ASSET_PATH  "..\\assets"

#include <opus.h>

#include "generated/sprites.h"
#include "particle_system.h"
#include "post_processing.h"

#include "particle_system.c"
#include "post_processing.c"
#include <opus.c>

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WORLD_WIDTH   960
#define WORLD_HEIGHT  540
// #define WORLD_WIDTH  640
// #define WORLD_HEIGHT 360

#define SORT_LAYER_INDEX_GROUND             2
#define SORT_LAYER_INDEX_SHADOW             3
#define SORT_LAYER_INDEX_BACKGROUND_EFFECTS 4
#define SORT_LAYER_INDEX_GAME               6
#define SORT_LAYER_INDEX_BULLETS            8
#define SORT_LAYER_INDEX_IN_GAME_EFFECTS    9
#define SORT_LAYER_INDEX_HUD                11
#define SORT_LAYER_INDEX_UI                 13
#define SORT_LAYER_INDEX_POST_GAME          15

typedef enum
{
    ColliderTypeNone,
    ColliderTypeEnemyHitbox,
    ColliderTypePlayerHitbox,
    ColliderTypePlayerAttack,
    ColliderTypeEnemyAttack,
    ColliderType_COUNT
} ColliderType;

/** entity */
typedef uint64 EntityProp;
enum
{
    EntityProp_RotateTowardsAim,
    EntityProp_RotateTowardsHeading,
    EntityProp_SmoothMovement,
    EntityProp_Lifetime,
    EntityProp_MarkedForDeletion,
    EntityProp_SimpleAI,
    EntityProp_CombatAI,
    EntityProp_PullTowardsPlayer,
    EntityProp_Bullet,
    EntityProp_Player,
    EntityProp_Collider,
    EntityProp_Attack,
    EntityProp_COUNT
};

typedef uint64            GameEntityIndex;
typedef struct GameEntity GameEntity;

struct GameEntity
{
    /** metadata */
    uint32 gen;
    uint64 props[(EntityProp_COUNT + 63) / 64];

    /** links */
    GameEntity* next;
    GameEntity* prev;
    GameEntity* parent;

    /** transform */
    Vec2    position;
    Vec2    scale;
    float32 rotation;

    /** combat */
    float32 health;
    float32 attack_rate;
    float32 t_attack;
    float32 damage;
    Vec2    bullet_spawn_offset;

    /** animation */
    float32    anim_scale_t;
    EasingType anim_scale_type;

    /** force */
    Vec2 force;
    Vec2 direction;

    /** loot */
    IRange coin_on_death;

    /** physics */
    ColliderType collider_type;
    float32      collider_radius;

    /** movement */
    Vec2    heading;
    Vec2    look_at;
    float32 speed;

    /** lifetime */
    float32 remaining_life;

    /** render info */
    Color       color;
    SpriteIndex sprite;

    float32        t_animation_clock;
    AnimationIndex animation;
    uint32         frame;
};

typedef struct
{
    /** engine */
    Arena*     persistent_arena;
    Arena*     frame_arena;
    Window*    window;
    EngineTime time;

    /** rendering */
    PassIndex     pass_post_processing;
    PassIndex     pass_pixel_perfect;
    PassIndex     pass_default;
    MaterialIndex material_post_processing;

    MaterialIndex material_pass_default;
    MaterialIndex material_projectile;

    /** physics */
    BitField64 collision_map[ColliderType_COUNT];

    /** input */
    InputMouse input_mouse;

    /** entities */
    GameEntity* free_entities;
    uint64      entity_count;
    GameEntity* first_entity;
    GameEntity* last_entity;

    /** game state */
    float32 t_spawn;

    /** editor */
    bool32 editor_active;
} GameState;
global GameState* g_state;

internal void g_init();

/** entity */
internal GameEntity* g_entity_alloc();
internal void        g_entity_free(GameEntity* e);
internal bool32      g_entity_has_prop(GameEntity* e, EntityProp prop);
internal void        g_entity_enable_prop(GameEntity* e, EntityProp prop);

/** render */
typedef struct
{
    Vec4    color;
    Vec4    outer_color;
    float32 fill_ratio;
    float32 slice_ratio;
} ShaderDataProjectile;
internal void draw_projectile(Vec2 pos, float32 radius, Color color);

/** utils */
internal GameEntity* g_spawn_enemy(Vec2 position);
internal GameEntity* g_spawn_bullet(Vec2 position, Vec2 direction, ColliderType collider_type, Color color, float32 size);

/** physics */
typedef struct
{
    ColliderType type;
    GameEntity*  entity;
    Vec2         position;
    float32      radius;
} ColliderInfo;

typedef struct
{
    GameEntity* a;
    GameEntity* b;
    Vec2        position;
} Collision;