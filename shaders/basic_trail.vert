#version 430 core

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

layout (std140, binding = 6) buffer Trail
{
    vec4 vertices[];
};

uniform mat4 u_model;
uniform sampler2D u_main_texture;

/* Vertex Data */
out vec2 v_tex_coord;

void main() 
{
    mat4 model = projection * view * u_model;
    gl_Position = model * vertices[gl_VertexID];
}