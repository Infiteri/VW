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
uniform float uDeltaTime;
uniform vec3 uCamPos;
out vec4 FragColor;

float hash(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

void main() {
    vec3 ray = normalize(vUVs);
    
    // sun position
    vec3 sunDir = normalize(vec3(sin(uTime * 0.3), 0.4, cos(uTime * 0.3)));
    float sunDot = dot(ray, sunDir);
    
    // horizon glow
    float horizon = smoothstep(-0.1, 0.3, ray.y);
    vec3 horizonCol = mix(vec3(0.8, 0.3, 0.1), vec3(0.2, 0.6, 1.0), horizon);
    
    // sun disk + halo
    vec3 sunGlow = vec3(1.0, 0.8, 0.4) * pow(max(0.0, sunDot), 128.0) * 3.0;
    sunGlow += vec3(1.0, 0.6, 0.2) * pow(max(0.0, sunDot), 8.0) * 0.5;
    
    // clouds
    float cloud = sin(ray.x * 5.0 + uTime * 0.3) * sin(ray.z * 5.0 - uTime * 0.2);
    cloud = smoothstep(0.3, 0.7, cloud + horizon * 0.5);
    
    // stars at night
    vec3 stars = vec3(hash(ray * 100.0)) * 0.4;
    stars *= smoothstep(0.95, 1.0, abs(sin(uTime * 0.5 + ray.x * 50.0)));
    stars *= smoothstep(0.2, -0.1, ray.y);  // only visible high up
    
    vec3 col = horizonCol;
    col += sunGlow;
    col += cloud * vec3(1.0) * 0.3;
    col += stars;
    
    FragColor = vec4(col, 1.0);
}
