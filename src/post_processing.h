#pragma once
#include <base.h>
#include <gfx.h>

#include <engine/time.h>

typedef struct
{
    Vec4 color;
    Vec4 aberration;
} ShaderDataPostProcessing;

typedef struct
{
    Renderer* renderer;

    /** settings */
    float32 camera_shake_cap;
    float32 camera_shake_decay_rate;

    /** state */
    float32 current_aberration_duration;
    float32 current_aberration_strength;

    float32 current_camera_shake_duration;
    float32 current_camera_shake_strength;

    ShaderDataPostProcessing* uniform_data;
} PostProcessingState;
global PostProcessingState* g_post_processing_state;

internal void post_processing_init(Arena* arena, Renderer* renderer);
internal void post_processing_update(EngineTime time);
internal void post_processing_add_aberration();
internal void post_processing_move_camera(Vec2 position, EngineTime time);
internal void post_processing_add_shake(float32 strength);