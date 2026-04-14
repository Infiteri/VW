// VERTEX
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 uProj;
uniform mat4 uView;

out vec3 vColor;

void main() {
    gl_Position = uProj * uView * vec4(aPosition, 1.0);
    vColor = aColor;
}

// FRAGMENT
#version 330 core

in vec3 vColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
