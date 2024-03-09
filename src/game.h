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

#include <opus.c>

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

typedef uint64 EntityProp;
enum
{
    EntityProp_RotateTowardsAim,
    EntityProp_RotateTowardsHeading,
    EntityProp_Lifetime,
    EntityProp_MarkedForDeletion,
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
    Color color;
};

typedef struct
{
    /** engine */
    Arena*     persistent_arena;
    Arena*     frame_arena;
    Window*    window;
    EngineTime time;

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