// VERTEX
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

layout(location = 4) in vec4 iRow0;
layout(location = 5) in vec4 iRow1;
layout(location = 6) in vec4 iRow2;
layout(location = 7) in vec4 iRow3;
layout(location = 8) in vec4 iColor;

uniform mat4 uProj;
uniform mat4 uView;

out vec4 vColor;
out vec2 vUV;

void main() {
    mat4 model = mat4(iRow0, iRow1, iRow2, iRow3);
    gl_Position = uProj * uView * model * vec4(aPosition, 1.0);
    vColor = iColor;
    vUV = aUV;
}

// FRAGMENT
#version 330 core

in vec4 vColor;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uTex;

void main() {
    FragColor = vColor;
    FragColor = texture2D(uTex, vUV);
}
