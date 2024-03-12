#include "post_processing.h"

internal void
post_processing_init(Arena* arena, Renderer* renderer)
{
    PostProcessingState* post_processing_state = arena_push_struct_zero(arena, PostProcessingState);
    post_processing_state->uniform_data        = arena_push_struct_zero(arena, ShaderDataPostProcessing);
    post_processing_state->renderer            = renderer;

    /** settings */
    post_processing_state->camera_shake_decay_rate = 0.9;
    post_processing_state->camera_shake_cap        = 0.9;

    g_post_processing_state = post_processing_state;
}

internal void
post_processing_update(EngineTime time)
{
    g_post_processing_state->uniform_data->aberration.x = (sin(time.current_frame_time / 40.0f) / 700.0f) * g_post_processing_state->current_aberration_strength;
    g_post_processing_state->uniform_data->aberration.y = (cos(time.current_frame_time / 20.0f) / 600.0f) * g_post_processing_state->current_aberration_strength;

    g_post_processing_state->current_aberration_duration -= time.dt;
    g_post_processing_state->current_aberration_duration = max(g_post_processing_state->current_aberration_duration, 0);
    g_post_processing_state->current_aberration_strength = powf(g_post_processing_state->current_aberration_duration, 2);

    g_post_processing_state->current_camera_shake_duration -= time.dt * g_post_processing_state->camera_shake_decay_rate;
    g_post_processing_state->current_camera_shake_duration = max(g_post_processing_state->current_camera_shake_duration, 0);
    g_post_processing_state->current_camera_shake_strength = powf(g_post_processing_state->current_camera_shake_duration, 2);
}

internal void
post_processing_move_camera(Vec2 position, EngineTime time)
{
    float32 x_shake = (sin(time.current_frame_time / 13.0f) * 1.18f) * g_post_processing_state->current_camera_shake_strength;
    float32 y_shake = (cos(time.current_frame_time / 15.0f) * 1.16f) * g_post_processing_state->current_camera_shake_strength;
    position.x += x_shake;
    position.y += y_shake;
    camera_move(g_post_processing_state->renderer, position);
}

internal void
post_processing_add_aberration()
{
    g_post_processing_state->current_aberration_duration = 1;
}

internal void
post_processing_add_shake(float32 strength)
{
    g_post_processing_state->current_camera_shake_duration = min(g_post_processing_state->current_camera_shake_duration + strength, g_post_processing_state->camera_shake_cap);
}