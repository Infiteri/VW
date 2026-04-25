// VERTEX
#version 400 core
#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 4) in vec4 iRow0;
layout(location = 5) in vec4 iRow1;
layout(location = 6) in vec4 iRow2;
layout(location = 7) in vec4 iRow3;
layout(location = 8) in vec4 iColor;
layout(location = 9) in uvec2 iAlbedo;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 vColor;
out vec2 vUV;
flat out uvec2 fAlbedo;

void main() {
    mat4 model = mat4(iRow0, iRow1, iRow2, iRow3);
    gl_Position = uProj * uView * model * vec4(aPosition, 1.0);
    vColor = iColor;
    vUV = aUV;
    fAlbedo = iAlbedo;
}

// FRAGMENT
#version 400 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

#define RENDER_MODE_FULL 0
#define RENDER_MODE_UV 1

uniform int uRenderMode;

in vec4 vColor;
in vec2 vUV;
flat in uvec2 fAlbedo;

out vec4 FragColor;

vec4 FullColor() {
    vec4 o;
    sampler2D s = sampler2D(packUint2x32(fAlbedo));
    o = texture(s, vUV);
    return o;
}

vec4 UVColor() {
    return vec4(vUV, 0, 1);
}

void main() {
    switch(uRenderMode) {
        case RENDER_MODE_FULL:
        default:
            FragColor = FullColor();
        break;

        case RENDER_MODE_UV:
            FragColor = UVColor();
        break;
    }
}
