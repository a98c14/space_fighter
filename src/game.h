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
#include "post_processing.h"

#include "post_processing.c"
#include <opus.c>

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

typedef uint64 EntityProp;
enum
{
    EntityProp_RotateTowardsAim,
    EntityProp_RotateTowardsHeading,
    EntityProp_Lifetime,
    EntityProp_MarkedForDeletion,
    EntityProp_Sprite,
    EntityProp_Bullet,
    EntityProp_Player,
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

    /** movement */
    Vec2    heading;
    Vec2    look_at;
    float32 speed;

    /** lifetime */
    float32 remaining_life;

    /** render info */
    Color       color;
    SpriteIndex sprite;
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

    /** input */
    InputMouse input_mouse;

    /** entities */
    GameEntity* free_entities;
    uint64      entity_count;
    GameEntity* first_entity;
    GameEntity* last_entity;

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
internal void draw_projectile(Vec2 pos, float32 radius);
