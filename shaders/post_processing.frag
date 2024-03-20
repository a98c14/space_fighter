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
    vec4 u_aberration;
    float u_saturation;
};

uniform mat4 u_model;
uniform sampler2D u_main_texture;

/* Vertex Data */
in vec2 v_tex_coord;

out vec4 color;

#define SHOW_NOISE 0
#define SRGB 0
// 0: Addition, 1: Screen, 2: Overlay, 3: Soft Light, 4: Lighten-Only
#define BLEND_MODE 0
#define SPEED 2.0
#define INTENSITY 0.055
// What gray level noise should tend to.
#define MEAN 0.0
// Controls the contrast/variance of noise.
#define VARIANCE 0.5

float gaussian(float z, float u, float o) {
    return (1.0 / (o * sqrt(2.0 * 3.1415))) * exp(-(((z - u) * (z - u)) / (2.0 * (o * o))));
}

vec3 inv_lerp(vec3 a, vec3 b, vec3 t)
{
    return (t - a) / (b - a);
}

void main() {
    vec2 uv = v_tex_coord;
    
    vec4 tex_color = texture(u_main_texture, uv);
    float tex_color_r = texture(u_main_texture, vec2(uv.x + u_aberration.x, uv.y + u_aberration.y)).r;
    // float tex_color_g = texture(u_main_texture, vec2(uv.x + u_aberration.y, uv.y)).g;
    float tex_color_b = texture(u_main_texture, vec2(uv.x + u_aberration.y, uv.y + u_aberration.z)).b;
    color = vec4(tex_color_r, tex_color.g, tex_color_b, tex_color.a);
    
    // TODO(selim): add LUT color grading
    /** post_processing: color grading */
    color.r *= 1.12;
    color.g *= 1.04;
    color.b *= 1.03;
    
    /** post_processing: vignette */
    float vignette_strength = pow(distance(uv, vec2(0.5, 0.5)), 1.3) * 1.2;
    vec4 vignette_color = vec4(0, 0, 0, 1);
    color = mix(color, vignette_color, vignette_strength);

    /** post_processing: noise [reference](https://www.shadertoy.com/view/4t2fRz)*/    
    float w = float(INTENSITY);
    float seed = dot(uv, vec2(12.9898, 78.233));
    float noise = fract(sin(seed) * 43758.5453 + g_time.x);
    noise = gaussian(noise, float(MEAN), float(VARIANCE) * float(VARIANCE));
    vec3 grain = vec3(noise) * (1.0 - color.rgb);
    color.rgb += grain * w;
    float grayscale = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
    color.rgb = mix(vec3(grayscale), color.rgb, u_saturation);
}
