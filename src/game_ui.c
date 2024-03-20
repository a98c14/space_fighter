#include "game_ui.h"
#include "game.h"

internal void
game_hud_update()
{
    profiler_begin("draw hud");
    draw_context_push(SORT_LAYER_INDEX_HUD, ViewTypeScreen, g_state->pass_default);
    Rect screen = screen_rect();
    // draw_rect(rect_cut_top(&screen, 20), ColorWhite);
    draw_context_pop();
    profiler_end();
}