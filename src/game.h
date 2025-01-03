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

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
// #define WORLD_WIDTH   960
// #define WORLD_HEIGHT  540
#define WORLD_WIDTH  640
#define WORLD_HEIGHT 360

#define SORT_LAYER_INDEX_GROUND             2
#define SORT_LAYER_INDEX_SHADOW             3
#define SORT_LAYER_INDEX_BACKGROUND_EFFECTS 4
#define SORT_LAYER_INDEX_GAME               6
#define SORT_LAYER_INDEX_BULLETS            8
#define SORT_LAYER_INDEX_IN_GAME_EFFECTS    9
#define SORT_LAYER_INDEX_HUD                11
#define SORT_LAYER_INDEX_UI                 13
#define SORT_LAYER_INDEX_POST_GAME          15

#define MAX_LEVEL_COUNT 8

typedef struct
{
    SpriteIndex sprite;
    String      description;

    /** features */
    float32 speed_increase;
    float32 damage_increase;
    int32   additional_projectile_count;
} GameSkill;

typedef enum
{
    ColliderTypeNone,
    ColliderTypeEnemyHitbox,
    ColliderTypePlayerHitbox,
    ColliderTypePlayerAttack,
    ColliderTypeEnemyAttack,
    ColliderType_COUNT
} ColliderType;

typedef enum
{
    GameStateFlagNone    = 0,
    GameStateFlagPaused  = 1 << 0,
    GameStateFlagLevelUp = 1 << 1,
    GameStateFlagEditor  = 1 << 2,
} GameStateFlag;

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
    EntityProp_DestroyWhenAwayFromPlayer,
    EntityProp_Player,
    EntityProp_Collider,
    EntityProp_Attack,
    EntityProp_COUNT
};

typedef struct
{
    SpriteIndex sprite;
    Vec2        position;
    float32     parallax_scale;
    float32     rotation;
} BackgroundObject;

typedef uint64            GameEntityIndex;
typedef struct GameEntity GameEntity;

typedef struct
{
    uint64 index;
    uint32 gen;
} GameEntityHandle;

// TODO(selim): static data about entities can be moved to a configuration struct outside of entitiy list
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
    float32 t_invulnerability;
    float32 invulnerability_duration;
    float32 damage;
    Vec2    bullet_spawn_offset;
    float32 bullet_speed;
    uint32  projectile_count;
    float32 t_out_of_range;

    /** animation */
    Vec2       anim_scale_start;
    Vec2       anim_scale_end;
    float32    anim_scale_t;
    float32    anim_scale_duration;
    EasingType anim_scale_easing;

    Vec4       anim_color_start;
    Vec4       anim_color_end;
    float32    anim_color_t;
    float32    anim_color_duration;
    EasingType anim_color_easing;

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
    float32 angular_speed;

    /** lifetime */
    float32 remaining_life;
    float32 t_alive;

    /** render info */
    Color       color;
    SpriteIndex sprite;

    float32        t_animation_clock;
    AnimationIndex animation;
    AnimationIndex on_delete_animation;
    uint32         frame;
};

read_only global GameEntity g_entity_nil = {0};

typedef struct
{
    /** engine */
    Arena*     persistent_arena;
    Arena*     frame_arena;
    Window*    window;
    EngineTime time;

    /** rendering */
    PassIndex pass_post_processing;
    PassIndex pass_pixel_perfect;
    PassIndex pass_default;

    MaterialIndex material_post_processing;
    MaterialIndex material_pass_default;
    MaterialIndex material_projectile;

    Vec2 camera_position;

    uint64            background_object_count;
    BackgroundObject* background_objects;

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
    GameStateFlag flags;
    float32       t_spawn;

    uint32 player_level;
    uint64 player_experience;
    uint64 experience_requirement[MAX_LEVEL_COUNT];

    uint8   player_skill_count;
    uint64* player_skills;

    /** data */
    uint32     skill_count;
    GameSkill* skills;
} GameState;
global GameState* g_state;

internal void g_init();

/* -------------------------------------------------------------------------- */
/*                                   ENTITY                                   */
/* -------------------------------------------------------------------------- */
internal GameEntity*      g_entity_alloc();
internal GameEntity*      g_entity_from_handle(GameEntityHandle handle);
internal GameEntityHandle g_handle_from_entity(GameEntity* entity);
internal void             g_entity_free(GameEntity* e);
internal bool32           g_entity_has_prop(GameEntity* e, EntityProp prop);
internal void             g_entity_enable_prop(GameEntity* e, EntityProp prop);

/* -------------------------------------------------------------------------- */
/*                                   RENDER                                   */
/* -------------------------------------------------------------------------- */
typedef struct
{
    Vec4    color;
    Vec4    outer_color;
    float32 fill_ratio;
    float32 slice_ratio;
} ShaderDataProjectile;
internal void draw_projectile(Vec2 pos, float32 radius, Color color);

/* -------------------------------------------------------------------------- */
/*                                  CONTROLS                                  */
/* -------------------------------------------------------------------------- */
typedef enum
{
    GameKeyNone = 0,
    GameKeyPause,
    GameKeyEditor,
} GameKey;

/* -------------------------------------------------------------------------- */
/*                                    UTILS                                   */
/* -------------------------------------------------------------------------- */
internal bool32      g_state_enabled(GameStateFlag flag);
internal void        g_state_toggle(GameStateFlag flag);
internal void        g_state_enable(GameStateFlag flag);
internal void        g_state_disable(GameStateFlag flag);
internal GameEntity* g_spawn_enemy(Vec2 position);
internal GameEntity* g_spawn_bullet(Vec2 position, Vec2 direction, ColliderType collider_type, Color color, float32 size, float32 speed, AnimationIndex on_delete_animation);

/* -------------------------------------------------------------------------- */
/*                              COMPONENT HELPERS                             */
/* -------------------------------------------------------------------------- */
internal void entity_set_color(GameEntity* entity, Color color);
internal void entity_set_color_animation(GameEntity* entity, Color start, Color end, float32 duration, EasingType easing);
internal void entity_set_scale(GameEntity* entity, Vec2 scale);
internal void entity_set_scale_animation(GameEntity* entity, Vec2 start, Vec2 end, float32 duration, EasingType easing);
internal void entity_add_force(GameEntity* entity, Vec2 force);

/* -------------------------------------------------------------------------- */
/*                                   PHYSICS                                  */
/* -------------------------------------------------------------------------- */
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