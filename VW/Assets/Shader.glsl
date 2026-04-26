// VERTEX
#version 400 core
#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 6)  in vec4 iRow0;
layout(location = 7)  in vec4 iRow1;
layout(location = 8)  in vec4 iRow2;
layout(location = 9)  in vec4 iRow3;
layout(location = 10) in vec4 iColor;
layout(location = 11) in uvec2 iAlbedo;
layout(location = 12) in uvec2 iNormal;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 vColor;
out vec2 vUV;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;

flat out uvec2 fAlbedo;
flat out uvec2 fNormal;

void main() {
    mat4 model = mat4(iRow0, iRow1, iRow2, iRow3);
    gl_Position = uProj * uView * model * vec4(aPosition, 1.0);
    vColor = iColor;

    vUV = aUV;
    vNormal = aNormal;
    vTangent = aTangent;
    vBitangent = aBitangent;
    
    fAlbedo = iAlbedo;
    fNormal = iNormal;
}

// FRAGMENT
#version 400 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

#define RENDER_MODE_FULL      0
#define RENDER_MODE_UV        1
#define RENDER_MODE_NORMAL    2
#define RENDER_MODE_TANGENT   3
#define RENDER_MODE_BITANGENT 4

uniform int uRenderMode;

in vec2 vUV;
in vec4 vColor;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;

flat in uvec2 fAlbedo;
flat in uvec2 fNormal;

out vec4 FragColor;

void main()
{
    switch(uRenderMode)
    {
        default:
        case RENDER_MODE_FULL:
        {
            sampler2D s = sampler2D(packUint2x32(fNormal));
            FragColor = texture(s, vUV) * vColor;
            break;
        }
        case RENDER_MODE_UV:
            FragColor = vec4(vUV, 0.0, 1.0);
            break;
        case RENDER_MODE_NORMAL:
            FragColor = vec4(vNormal * 0.5 + 0.5, 1.0);
            break;
        case RENDER_MODE_TANGENT:
            FragColor = vec4(vTangent * 0.5 + 0.5, 1.0);
            break;
        case RENDER_MODE_BITANGENT:
            FragColor = vec4(vBitangent * 0.5 + 0.5, 1.0);
            break;
    }
}
