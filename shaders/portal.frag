#version 430 core
#define PI 3.14159

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

float expStep( float x, float k, float n ){
    return exp( -k*pow(x,n) );
}

mat2 rot(float rads)
{
    return mat2(cos(rads), sin(rads), -sin(rads), cos(rads));
}

/* Vertex Data */
in vec2 v_tex_coord;

out vec4 color;

void main() {
    vec2 uv = v_tex_coord;
    uv = uv * 2.0 - 1.0;
    vec2 p = uv;
    p = rot(g_time.x * 1.25) * p;
    p = vec2(p.x, -p.y) + .15;
    
    float r = length(p);
    float a = atan(p.y, p.x);
    a += 2. * sin(a);
    float coord = fract(a / PI + expStep(r, 1., .5) * 8. + 1.6 * g_time.x);
    vec3 col = mix(vec3(.17, 0., .25), vec3(.3, 0., .5), step(.6, coord));

    float d = 1 - distance(uv, vec2(0));
    float d_outline = step(0.2, sin(g_time.x)/10.0+d + 0.1);
    d = step(0.2, sin(g_time.x)/10.0+d);

    col *= pow(r, .65) * 1.75;
    if(d > 0)
        color = vec4(col, d);
    else if(d_outline > 0)
        color = vec4(0.2,0.1,0.3, 1.0);
    else
        color = vec4(0.0);
}
