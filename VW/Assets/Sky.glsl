// VERTEX
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vUVs;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
    gl_Position = uProj * uView *  vec4(aPosition, 1.0);
    vUVs = aPosition;
}

// FRAGMENT
#version 330 core
in vec3 vUVs;
uniform float uTime;
uniform vec3 uCamPos;
out vec4 FragColor;

void main()
{
    vec3 ray = normalize(vUVs);

    // calm zenith -> horizon gradient
    float height = max(ray.y, 0.0);
    vec3 zenith = vec3(0.05, 0.06, 0.12);
    vec3 horizon = vec3(0.15, 0.12, 0.18);
    vec3 col = mix(horizon, zenith, sqrt(height));

    // very subtle warm glow near sun direction
    vec3 sunDir = normalize(vec3(sin(uTime * 0.15), 0.35, cos(uTime * 0.15)));
    float sunDot = max(dot(ray, sunDir), 0.0);
    col += vec3(0.3, 0.2, 0.1) * pow(sunDot, 64.0) * 0.08;

    FragColor = vec4(col, 1.0);
}
