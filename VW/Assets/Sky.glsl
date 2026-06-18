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
uniform vec4 uColor;
uniform float uIntensity;
uniform vec2 uTiling;
uniform vec3 uOffset;
uniform vec4 uClipPlane;
uniform int uSampleCount;
uniform float uMass;

out vec4 FragColor;

float hash(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43.5453);
}

float star(vec3 p) {
    vec3 i = floor(p);
    float n = dot(i, vec3(12.9898, 78.233, 45.543));
    return smoothstep(0.997, 1.0, fract(sin(n) * 43758.5453));
}

vec3 traceRay(vec3 origin, vec3 dir, float rs, float inner, float outer) {
    int steps = max(uSampleCount, 64);
    float near = uClipPlane.x;
    float far = uClipPlane.w;
    float dt = (far - near) / float(steps);
    float warp = uClipPlane.y;

    vec3 ray = dir;
    vec3 col = vec3(0.0);
    float alpha = 0.0;
    vec3 prev = origin + ray * near;
    float t = near;

    for (int i = 0; i < steps; i++) {
        t += dt;
        if (t > far || alpha > 0.99) break;

        vec3 pos = origin + ray * t;
        float r = length(pos);

        float bend = warp * rs / (r * r + 0.5);
        ray = normalize(ray + (-normalize(pos)) * bend);
        pos = origin + ray * t;
        r = length(pos);

        if (r < rs * 0.9) {
            col = vec3(0.0);
            alpha = 1.0;
            break;
        }

        if (sign(prev.y) != sign(pos.y)) {
            float frac = prev.y / (prev.y - pos.y);
            vec3 hit = mix(prev, pos, frac);
            float dr = length(hit.xz);

            if (dr > inner && dr < outer) {
                float norm = (dr - inner) / (outer - inner);
                float temp = 1.0 - norm;

                vec3 c1 = vec3(1.0, 0.9, 0.6);
                vec3 c2 = vec3(1.0, 0.3, 0.0);
                vec3 c3 = vec3(0.4, 0.05, 0.0);
                vec3 diskCol = norm < 0.5
                    ? mix(c1, c2, norm * 2.0)
                    : mix(c2, c3, (norm - 0.5) * 2.0);

                float bright = pow(temp, 2.5) * 3.0;

                float swirl = fract(atan(hit.z, hit.x) / 6.2832 + uTime * 0.1 + dr * 0.3);
                bright *= 0.7 + 0.3 * smoothstep(0.0, 0.15, swirl) * smoothstep(0.3, 0.15, swirl);

                vec3 tang = normalize(cross(vec3(0,1,0), normalize(hit)));
                float dop = 1.0 + dot(ray, tang) * 0.5;
                diskCol *= dop;

                float a = min(bright * 0.6, 1.0 - alpha);
                col += diskCol * bright * a;
                alpha += a;
            }
        }

        float ringR = abs(r - rs * 1.5);
        if (ringR < rs * 0.08) {
            float ring = pow(1.0 - ringR / (rs * 0.08), 3.0);
            col += vec3(1.0, 0.85, 0.5) * ring * 0.4 * (1.0 - alpha);
        }

        prev = pos;
    }

    return col;
}

void main() {
    vec3 dir = normalize(vUVs);
    vec3 origin = uCamPos + uOffset;

    float rs = max(uMass * 2.0, 0.1);
    float inner = uTiling.x * rs;
    float outer = uTiling.y * rs;

    vec3 sp = dir * 600.0;
    vec3 bg = vec3(0.005, 0.003, 0.012);
    bg += star(sp) * vec3(0.9, 0.95, 1.0) * 0.6;
    bg += star(sp * 1.7 + 30.0) * vec3(1.0, 0.85, 0.7) * 0.4;
    bg += star(sp * 2.3 + 70.0) * vec3(0.8, 0.9, 1.0) * 0.3;

    float rim = length(dir.xz);
    bg += vec3(0.01, 0.005, 0.02) * smoothstep(0.9, 0.0, abs(dir.y));

    vec3 bh = traceRay(origin, dir, rs, inner, outer);
    vec3 final = bg + bh;
    final *= uColor.rgb * uIntensity;
    final = final / (final + vec3(1.0));
    final = pow(final, vec3(1.0 / 2.2));

    FragColor = vec4(final, 1.0);
}
