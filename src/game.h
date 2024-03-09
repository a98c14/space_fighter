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

typedef struct
{
    /** engine */
    Arena*     persistent_arena;
    Arena*     frame_arena;
    Window*    window;
    EngineTime time;

} GameState;
global GameState* g_state;

internal void game_init();