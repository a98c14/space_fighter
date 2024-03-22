#include "game_ui.h"
#include "game.h"

internal void
game_ui_state_init(Arena* arena)
{
    g_game_ui_state             = arena_push_struct_zero(arena, GameUIState);
    g_game_ui_state->arena      = arena;
    g_game_ui_state->ui_box_map = arena_push_array_zero(arena, GameUIBoxBucket, GAME_UI_BOX_TABLE_SIZE);
}

internal void
game_ui_update()
{
    profiler_begin("ui update");
    ArenaTemp temp = scratch_begin(0, 0);
    float32   dt   = g_state->time.dt;

    /** ui draw hud */
    profiler_scope("ui draw hud")
    {
        draw_context_push(SORT_LAYER_INDEX_HUD, ViewTypeScreen, g_state->pass_default);
        Rect screen = screen_rect();
        if (g_state_enabled(GameStateFlagLevelUp))
        {
            Rect    container            = rect_shrink(screen, vec2(200, 120));
            float32 card_container_width = container.w / 3.0f;

            for (uint32 i = 0; i < g_state->skill_count; i++)
            {
                if (game_hud_draw_level_up_card(ui_key_str(string_pushf(temp.arena, "skill_box_%d", i)), rect_cut_left(&container, card_container_width), g_state->skills[i]).pressed)
                {
                    g_state_disable(GameStateFlagLevelUp | GameStateFlagPaused);

                    log_info("pressed skill %d", i);
                }
            }
        }
        g_state->background_object_count;
        draw_context_pop();
    }

    /** ui state updates */
    profiler_scope("ui state update")
    {
    }
    scratch_end(temp);
    profiler_end();
}

internal GameUIBox*
game_ui_box_from_key(UI_Key key)
{
    GameUIBoxBucket* bucket = &g_game_ui_state->ui_box_map[key.value % GAME_UI_BOX_TABLE_SIZE];

    GameUIBoxNode* node;
    for_each(node, bucket->first)
    {
        if (node->v.key.value == key.value)
            return &node->v;
    }

    node        = arena_push_struct_zero(g_game_ui_state->arena, GameUIBoxNode);
    node->v.key = key;

    dll_push_back(bucket->first, bucket->last, node);
    return &node->v;
}

internal GameUISignal
game_hud_draw_level_up_card(UI_Key key, Rect rect, GameSkill skill)
{
    GameUISignal result = {0};
    GameUIBox*   ui_box = game_ui_box_from_key(key);
    bool32       is_hot = ui_key_same(key, g_game_ui_state->hot);
    if (!is_hot && intersects_rect_point(rect, g_state->input_mouse.screen).intersects)
    {
        g_game_ui_state->hot = key;
        ui_box->t_hot        = g_state->time.current_frame_time;
    }

    if (is_hot && input_mouse_pressed(g_state->input_mouse, MouseButtonStateLeft))
    {
        g_game_ui_state->active = key;
        ui_box->t_active        = g_state->time.current_frame_time;
    }

    if (is_hot && input_mouse_released(g_state->input_mouse, MouseButtonStateLeft))
    {
        result.pressed = true;
    }

    // TODO(selim): we can `lerp` here
    ui_box->rect = rect;

    const uint32 card_header_length = 55;
    const uint32 card_header_icon_x = 46;
    const uint32 card_header_icon_w = 38;

    ArenaTemp temp = scratch_begin(0, 0);
    scratch_end(temp);
    float32 alpha_t       = is_hot ? clamp(0, (g_state->time.current_frame_time - ui_box->t_hot) / 100.0f, 1) : 0;
    Color   overlay_color = lerp_color(ColorBlackA, ColorInvisible, alpha_t);

    Rect card_container = draw_sprite_rect(ui_box->rect, SPRITE_GAME_UI_UPGRADE_UI_CARD, ANCHOR_C_C);
    Rect overlay_rect   = rect_expand(card_container, vec2(2, 2));

    /** header */
    Rect rect_header = rect_cut_top(&card_container, card_header_length);
    rect_cut_left(&rect_header, card_header_icon_x);
    draw_sprite_rect(rect_cut_left(&rect_header, card_header_icon_w), skill.sprite, ANCHOR_C_C);

    /** description */
    Rect rect_description = rect_shrink(card_container, vec2(8, 8));
    draw_text(skill.description, rect_description, ANCHOR_TL_TL, 7, ColorWhite);
    draw_rect(overlay_rect, overlay_color);

    return result;
}
