#include "particle_system.h"

internal void
ps_init(Arena* arena, Arena* frame_arena, SpriteAtlas* atlas, uint64 capacity, PassIndex default_pass)
{
    ps_state = arena_push_struct_zero(arena, PS_State);

    ps_state->particle_buffer         = arena_push_array_zero(arena, PS_Particle, capacity);
    ps_state->particle_kind_buffer    = arena_push_array_zero(arena, PS_ParticleKind, capacity);
    ps_state->effect_scale_buffer     = arena_push_array_zero(arena, PS_EffectScaleAnimation, capacity);
    ps_state->effect_color_buffer     = arena_push_array_zero(arena, PS_EffectColorAnimation, capacity);
    ps_state->effect_direction_buffer = arena_push_array_zero(arena, PS_EffectDirection, capacity);
    ps_state->temp_arena              = frame_arena;
    ps_state->particle_capacity       = capacity;
    ps_state->atlas                   = atlas;
    ps_state->animation_update_rate   = 1.0f / 24.0f;
    ps_state->default_pass            = default_pass;
}

internal void
ps_update(float32 dt)
{
    profiler_begin_color("particle system", ColorYellowPastel);
    PS_PassRenderGroup render_groups[PASS_CAPACITY] = {0};
    for (uint32 i = 0; i < PASS_CAPACITY; i++)
    {
        render_groups[i].requests = arena_push_array(ps_state->temp_arena, SpriteRenderRequest, ps_state->active_particle_count_per_pass[i]);
    }

    /** update particles */
    profiler_scope("particle system state update") for (uint64 i = ps_state->particle_start; i < ps_state->particle_end; i++)
    {
        uint64          particle_internal_index = i % ps_state->particle_capacity;
        PS_ParticleKind particle_kind           = ps_state->particle_kind_buffer[particle_internal_index];
        if (particle_kind == PS_ParticleKindDead)
            continue;

        PS_Particle* particle = &ps_state->particle_buffer[particle_internal_index];
        if (particle->delay_t > 0)
        {
            particle->delay_t -= dt;
            continue;
        }

        float32 lifetime = particle->total_t - particle->remaining_t;
        float32 t        = lifetime / particle->total_t;

        SpriteRenderRequest render_info = {0};
        render_info.rotation            = particle->rotation;
        render_info.layer               = particle->sort_layer;

        // TODO(selim): Add outline color to particle, the issue is only the text particles use it at the moment
        Color outline_color = {0};
        /** set color */
        if (particle->effects & PS_EffectFlagsColorAnimation)
        {
            PS_EffectColorAnimation* effect = &ps_state->effect_color_buffer[particle_internal_index];
            render_info.color               = vec4_to_color(lerp_vec4(particle->starting_color, effect->color, ease_dynamic(t, effect->easing)));
            outline_color                   = vec4_to_color(lerp_vec4(d_color_black, effect->outline_color, ease_dynamic(t, effect->easing)));
        }
        else
        {
            render_info.color = vec4_to_color(particle->starting_color);
            outline_color     = vec4_to_color(particle->starting_color);
        }

        /** set scale */
        float32 scale;
        if (particle->effects & PS_EffectFlagsScaleAnimation)
        {
            PS_EffectScaleAnimation* effect = &ps_state->effect_scale_buffer[particle_internal_index];
            scale                           = lerp_f32(particle->starting_scale, effect->target_scale, ease_dynamic(t, effect->easing));
        }
        else
        {
            scale = particle->starting_scale;
        }

        /** set position */
        if (particle->effects & PS_EffectFlagsDirection)
        {
            PS_EffectDirection* effect = &ps_state->effect_direction_buffer[particle_internal_index];
            render_info.position       = lerp_vec3(particle->starting_position, add_vec3(particle->starting_position, effect->direction), ease_dynamic(t, effect->easing));
        }
        else
        {
            render_info.position = particle->starting_position;
        }
        render_info.sort_order = -1 * render_info.position.y / 10.0f;
        render_info.scale      = vec2(scale, scale);

        /** kind specific settings & render */
        switch (particle_kind)
        {
        case PS_ParticleKindSprite:
        {
            render_info.sprite = particle->sprite;
            ps_render_group_add(render_groups, particle->pass, particle->sort_layer, render_info);
            break;
        }
        case PS_ParticleKindAnimation:
        {
            int32 frame = lifetime / ps_state->animation_update_rate;

            const Animation* animation = &ps_state->atlas->animations[particle->animation];
            render_info.sprite         = animation->sprite_start_index + frame;
            ps_render_group_add(render_groups, particle->pass, particle->sort_layer, render_info);
            break;
        }
        case PS_ParticleKindText:
        {
            draw_text_at_internal(string_create(particle->text.value, particle->text.length), render_info.position.xy, AlignmentCenter, scale, render_info.color);
            break;
        }
        default:
            not_implemented();
        }

        /** clean up */
        particle->remaining_t -= dt;
        if (particle->remaining_t <= 0)
        {
            // TODO(selim): move sort mechanism to renderer itself and remove this nonsense
            if (particle_kind != PS_ParticleKindText)
                ps_state->active_particle_count_per_pass[particle->pass]--;

            ps_state->particle_kind_buffer[particle_internal_index] = PS_ParticleKindDead;

            // if this is the first particle since it is now dead, loop until start index reaches the first alive particle
            if (i == ps_state->particle_start)
            {
                while (i < ps_state->particle_end && ps_state->particle_kind_buffer[i % ps_state->particle_capacity] == PS_ParticleKindDead)
                {
                    ps_state->particle_start++;
                    i++;
                }
            }
        }
    }

    /** render particles */
    profiler_scope("particle system render") for (uint32 i = 0; i < PASS_CAPACITY; i++)
    {
        render_sprites_sorted(ps_state->temp_arena, i, render_groups[i].requests, render_groups[i].render_count, render_groups[i].layer_counts);
    }

    profiler_end();
}

internal void
ps_render_group_add(PS_PassRenderGroup* pass_groups, PassIndex pass, SortLayerIndex sort_layer, SpriteRenderRequest request)
{
    PS_PassRenderGroup* group            = &pass_groups[pass];
    group->requests[group->render_count] = request;
    group->render_count++;
    group->layer_counts[sort_layer]++;
}

internal ParticleIndex
ps_particle_new_internal(PS_ParticleKind kind, Vec3 position, float32 duration, PassIndex pass, SortLayerIndex sort_layer)
{
    uint64 particle_index = ps_state->particle_end;

    ps_state->particle_kind_buffer[particle_index % ps_state->particle_capacity] = kind;

    PS_Particle* p = &ps_state->particle_buffer[particle_index % ps_state->particle_capacity];
    p->effects     = 0;
    p->sort_layer  = sort_layer;
    p->pass        = pass;
    p->remaining_t = duration;
    p->total_t     = duration;

    p->starting_color    = color_v4(ColorInvisible);
    p->starting_position = position;
    p->starting_scale    = 1;
    ps_state->particle_end++;
    ps_state->active_particle_count_per_pass[pass]++;
    return particle_index;
}

internal ParticleIndex
ps_particle_animation(Vec3 position, AnimationIndex animation, float32 rotation)
{
    float32      duration       = animation_length(ps_state->atlas->animations[animation]) * ps_state->animation_update_rate;
    uint64       particle_index = ps_particle_new_internal(PS_ParticleKindAnimation, position, duration, ps_state->default_pass, SORT_LAYER_INDEX_IN_GAME_EFFECTS);
    PS_Particle* p              = &ps_state->particle_buffer[particle_index % ps_state->particle_capacity];
    p->animation                = animation;
    p->rotation                 = rotation;
    return particle_index;
}

internal ParticleIndex
ps_particle_sprite(Vec3 position, SpriteIndex sprite, float32 duration)
{
    uint64       particle_index = ps_particle_new_internal(PS_ParticleKindSprite, position, duration, ps_state->default_pass, SORT_LAYER_INDEX_GAME);
    PS_Particle* p              = &ps_state->particle_buffer[particle_index % ps_state->particle_capacity];
    p->sprite                   = sprite;
    return particle_index;
}

internal ParticleIndex
ps_particle_text(Vec3 position, String str, float32 duration, Color color, float32 size)
{
    uint64       particle_index = ps_particle_new_internal(PS_ParticleKindText, position, duration, ps_state->default_pass, SORT_LAYER_INDEX_IN_GAME_EFFECTS);
    PS_Particle* p              = &ps_state->particle_buffer[particle_index % ps_state->particle_capacity];
    p->starting_color           = color_v4(color);
    p->starting_scale           = size;
    p->text.length              = min(str.length, 4);
    memcpy(p->text.value, str.value, p->text.length);
    // HACK(selim): will be removed once we move the sorting to renderer
    ps_state->active_particle_count_per_pass[ps_state->default_pass]--;
    return particle_index;
}

internal ParticleIndex
ps_particle_pixel_perfect_sprite(Vec3 position, SpriteIndex sprite, float32 duration)
{
    uint64       particle_index = ps_particle_new_internal(PS_ParticleKindSprite, position, duration, ps_state->default_pass, SORT_LAYER_INDEX_SHADOW + 1);
    PS_Particle* p              = &ps_state->particle_buffer[particle_index % ps_state->particle_capacity];
    p->sprite                   = sprite;
    return particle_index;
}

internal ParticleIndex
ps_particle_animation_delayed(Vec3 position, AnimationIndex animation, float32 delay)
{
    ParticleIndex particle = ps_particle_animation(position, animation, 0);
    ps_set_delay(particle, delay);
    return particle;
}

internal void
ps_set_delay(ParticleIndex particle, float32 delay)
{
    PS_Particle* p = &ps_state->particle_buffer[particle % ps_state->particle_capacity];
    p->delay_t     = delay;
}

internal void
ps_set_scale_animation(ParticleIndex particle, EasingType easing, float32 size)
{
    PS_Particle* p                                                                     = &ps_state->particle_buffer[particle % ps_state->particle_capacity];
    p->effects                                                                         = p->effects | PS_EffectFlagsScaleAnimation;
    ps_state->effect_scale_buffer[particle % ps_state->particle_capacity].target_scale = size;
    ps_state->effect_scale_buffer[particle % ps_state->particle_capacity].easing       = easing;
}

internal void
ps_set_direction(ParticleIndex particle, EasingType easing, Vec3 v)
{
    PS_Particle* p                                                                      = &ps_state->particle_buffer[particle % ps_state->particle_capacity];
    p->effects                                                                          = p->effects | PS_EffectFlagsDirection;
    ps_state->effect_direction_buffer[particle % ps_state->particle_capacity].direction = v;
    ps_state->effect_direction_buffer[particle % ps_state->particle_capacity].easing    = easing;
}

internal void
ps_set_color_animation(ParticleIndex particle, EasingType easing, Color color)
{
    PS_Particle* p                                                               = &ps_state->particle_buffer[particle % ps_state->particle_capacity];
    p->effects                                                                   = p->effects | PS_EffectFlagsColorAnimation;
    ps_state->effect_color_buffer[particle % ps_state->particle_capacity].color  = color_v4(color);
    ps_state->effect_color_buffer[particle % ps_state->particle_capacity].easing = easing;
}