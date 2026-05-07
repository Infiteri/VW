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
out vec3 vWorldPos;

flat out uvec2 fAlbedo;
flat out uvec2 fNormal;

void main() {
    mat4 model = mat4(iRow0, iRow1, iRow2, iRow3);
    gl_Position = uProj * uView * model * vec4(aPosition, 1.0);
    vColor = iColor;
    vWorldPos = vec3(model * vec4(aPosition, 1.0));

    vUV = aUV;
    vNormal = aNormal;
    vTangent = aTangent;
    vBitangent = aBitangent;
    
    fAlbedo = iAlbedo;
    fNormal = iNormal;
}

// FRAGMENT
#version 430 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

#define RENDER_MODE_FULL      0
#define RENDER_MODE_UV        1
#define RENDER_MODE_NORMAL    2
#define RENDER_MODE_TANGENT   3
#define RENDER_MODE_BITANGENT 4

#define LIGHT_AMBIENT     0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_POINT       2
#define LIGHT_SPOT        3

struct GPULight
{
    vec4 Position;    // xyz=pos, w=range
    vec4 Direction;   // xyz=dir, w=outerCos
    vec4 Color;       // xyz=color, w=intensity
    vec4 Data;        // x=type, y=innerCos
    vec4 Attenuation; // xyz=const/lin/quad
};

layout(std430, binding = 3) buffer LightBuffer { GPULight lights[]; };

uniform int   uRenderMode;
uniform int   uLightCount;
uniform vec3  uCamPos;

in vec2 vUV;
in vec4 vColor;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vWorldPos;
flat in uvec2 fAlbedo;
flat in uvec2 fNormalMap;

out vec4 FragColor;

// PBR helpers
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a2    = roughness * roughness * roughness * roughness;
    float NdotH = max(dot(N, H), 0.0);
    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265359 * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    return GeometrySchlickGGX(max(dot(N, V), 0.0), roughness)
         * GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

vec3 PBR(vec3 albedo, vec3 N, vec3 V, vec3 L, vec3 radiance, float metallic, float roughness)
{
    vec3 H     = normalize(V + L);
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

    vec3  F0      = mix(vec3(0.04), albedo, metallic);
    vec3  F       = FresnelSchlick(max(dot(V, H), 0.0), F0);
    float D       = DistributionGGX(N, H, roughness);
    float G       = GeometrySmith(N, V, L, roughness);

    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.0001);
    vec3 kD       = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 diffuse  = kD * albedo / 3.14159265359;

    return (diffuse + specular) * radiance * NdotL;
}

vec4 CalculateFinalColor()
{
    sampler2D albedoSampler = sampler2D(packUint2x32(fAlbedo));
    vec4 albedoSample = texture(albedoSampler, vUV) * vColor;
    vec3 albedo = albedoSample.rgb;

    // normal map
    vec3 N = normalize(vNormal);
    if (fNormalMap.x != 0u || fNormalMap.y != 0u)
    {
        sampler2D normalSampler = sampler2D(packUint2x32(fNormalMap));
        vec3 n = texture(normalSampler, vUV).rgb * 2.0 - 1.0;
        mat3 TBN = mat3(normalize(vTangent), normalize(vBitangent), N);
        N = normalize(TBN * n);
    }

    vec3 V        = normalize(uCamPos - vWorldPos);
    float metallic  = 0.0; // TODO: from texture
    float roughness = 0.5; // TODO: from texture

    vec4 finalColor = vec4(0.0);

    for (int i = 0; i < uLightCount; i++)
    {
        GPULight light = lights[i];
        int type = int(light.Data.x);

        if (type == LIGHT_AMBIENT)
        {
            finalColor += vec4(albedo * light.Color.rgb * light.Color.w, albedoSample.a);
        }
        else if (type == LIGHT_DIRECTIONAL)
        {
            vec3 L       = normalize(-light.Direction.xyz);
            vec3 radiance = light.Color.rgb * light.Color.w;
            finalColor  += vec4(PBR(albedo, N, V, L, radiance, metallic, roughness), 0.0);
        }
        else if (type == LIGHT_POINT)
        {
            vec3  toLight = light.Position.xyz - vWorldPos;
            float dist    = length(toLight);
            if (dist > light.Position.w) continue;

            vec3  L       = toLight / dist;
            vec3  atten   = light.Attenuation.xyz;
            float attenuation = 1.0 / (atten.x + atten.y * dist + atten.z * dist * dist);
            vec3  radiance    = light.Color.rgb * light.Color.w * attenuation;
            finalColor       += vec4(PBR(albedo, N, V, L, radiance, metallic, roughness), 0.0);
        }
        else if (type == LIGHT_SPOT)
        {
            vec3  toLight = light.Position.xyz - vWorldPos;
            float dist    = length(toLight);
            if (dist > light.Position.w) continue;

            vec3  L          = toLight / dist;
            float cosAngle   = dot(-L, normalize(light.Direction.xyz));
            float innerCos   = light.Data.y;
            float outerCos   = light.Direction.w;
            float spotFactor = smoothstep(outerCos, innerCos, cosAngle);
            if (spotFactor <= 0.0) continue;

            vec3  atten       = light.Attenuation.xyz;
            float attenuation = 1.0 / (atten.x + atten.y * dist + atten.z * dist * dist);
            vec3  radiance    = light.Color.rgb * light.Color.w * attenuation * spotFactor;
            finalColor       += vec4(PBR(albedo, N, V, L, radiance, metallic, roughness), 0.0);
        }
    }

    return finalColor;
}

void main()
{
    switch(uRenderMode)
    {
        default:
        case RENDER_MODE_FULL:
            FragColor = CalculateFinalColor();
            break;
        case RENDER_MODE_UV:
            FragColor = vec4(vUV, 0.0, 1.0);
            break;
        case RENDER_MODE_NORMAL:
            FragColor = vec4(normalize(vNormal) * 0.5 + 0.5, 1.0);
            break;
        case RENDER_MODE_TANGENT:
            FragColor = vec4(normalize(vTangent) * 0.5 + 0.5, 1.0);
            break;
        case RENDER_MODE_BITANGENT:
            FragColor = vec4(normalize(vBitangent) * 0.5 + 0.5, 1.0);
            break;
    }
}
