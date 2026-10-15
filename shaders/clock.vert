#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 clockTime; // hours, minutes, seconds

const float PI = 3.14159265359;

mat3 rotateZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    return mat3(c, s, 0.0, -s, c, 0.0, 0.0, 0.0, 1.0);
}

void main() {
    float angle = 0.0;

    if (all(equal(color, vec3(0.0, 0.0, 1.0)))) {
        // Blue = hour
        angle
            = -2.0 * PI * (mod(clockTime.x, 12.0) + clockTime.y / 60.0) / 12.0;
    } else if (all(equal(color, vec3(0.0, 1.0, 0.0)))) {
        // Green = minute
        angle = -2.0 * PI * (clockTime.y + clockTime.z / 60.0) / 60.0;
    } else if (all(equal(color, vec3(1.0, 0.0, 0.0)))) {
        // Red = second
        angle = -2.0 * PI * clockTime.z / 60.0;
    }

    mat3 rotation = rotateZ(angle);

    vertexColor = color;
    vertexNormal = rotation * normal;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix
                * vec4(rotation * position, 1.0);
}
