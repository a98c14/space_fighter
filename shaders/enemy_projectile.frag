#version 430 core
#define PI 3.1415
struct ShaderData
{
    vec4 color;
    vec4 outer_color;
    float fill_ratio;
    float slice;
};

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coord;

layout (std140, binding = 0) uniform Global
{
    vec4 g_time;
};

layout (std140, binding = 1) uniform Texture
{
    vec2 texture_size;
    float texture_layer_count;
};

layout (std140, binding = 2) uniform Camera
{
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 3) buffer Matrices
{
    mat4 model[];
};

layout (std140, binding = 4) buffer Custom
{
    ShaderData data[];
};

uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;
flat in int v_instance_id;

out vec4 color;

void main() {
    ShaderData v_data = data[v_instance_id];
    vec2 uv = v_tex_coord;
    uv = uv * 2.0 - 1.0;

    float a = atan(uv.y, uv.x);
    float d = distance(uv, vec2(0.0));
    float d_inner_extra = abs(sin(g_time.x + a * 0.94)) / 20;
    float d_outer_extra = abs(cos(g_time.x + a * 0.9)) / 24;
    float enabled_inner = 1 - step(0.3 + d_inner_extra, d);
    float enabled_outer = 1 - step(0.5 + d_outer_extra, d);
    vec4 color_inner = vec4(v_data.color.xyz, enabled_inner);
    vec4 color_outer = vec4(v_data.outer_color.xyz, enabled_outer);
    color = mix(color_outer, color_inner, enabled_inner);
}