#pragma once
#include <base.h>
#include <gfx/sprite.h>

#include "game.h"

#define GAME_UI_BOX_TABLE_SIZE 256

typedef struct
{
    bool32 pressed;
} GameUISignal;

typedef struct
{
    UI_Key key;
    Rect   rect;

    float32 t_hot;
    float32 t_active;
} GameUIBox;

typedef struct GameUIBoxNode GameUIBoxNode;
struct GameUIBoxNode
{
    GameUIBox v;

    GameUIBoxNode* next;
    GameUIBoxNode* prev;
};

typedef struct
{
    GameUIBoxNode* first;
    GameUIBoxNode* last;
} GameUIBoxBucket;

typedef struct
{
    Arena* arena;

    GameUIBoxBucket* ui_box_map;
    UI_Key           active;
    UI_Key           hot;
} GameUIState;
global GameUIState* g_game_ui_state;

/* -------------------------------------------------------------------------- */
/*                                   UI BASE                                  */
/* -------------------------------------------------------------------------- */
internal void       game_ui_state_init(Arena* arena);
internal void       game_ui_update();
internal GameUIBox* game_ui_box_from_key(UI_Key key);
internal void       game_ui_signal();

/* -------------------------------------------------------------------------- */
/*                                   WIDGETS                                  */
/* -------------------------------------------------------------------------- */
internal GameUISignal game_hud_draw_level_up_card(UI_Key key, Rect rect, GameSkill skill);
