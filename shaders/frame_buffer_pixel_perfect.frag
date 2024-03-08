#version 430 core

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

layout (std140, binding = 4) uniform Custom
{
    vec4 u_color;
};

uniform mat4 u_model;
uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;

out vec4 color;


vec2 calculate_uv(vec2 uv, vec2 tex_size) 
{
    vec2 pixels = uv * tex_size;
    vec2 alpha = 0.7 * fwidth(pixels);
    vec2 pixels_fract = fract(pixels);
    vec2 pixels_diff = clamp( .5 / alpha * pixels_fract, 0.0, .5 ) + clamp( .5 / alpha * (pixels_fract - 1) + .5, 0.0, .5 );
    pixels = floor(pixels) + pixels_diff;
    return pixels / tex_size;
}


void main() {
    vec2 uv = calculate_uv(v_tex_coord, texture_size);
    vec4 tex_color = texture(u_main_texture, uv);
    color = tex_color;
}
