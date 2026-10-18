#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

flat out vec3 vertexColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 clockTime;

const float two_pi = 2 * 3.14159265;

void main() {
    float phase = 0.0;

    if (all(equal(color, vec3(0.0, 0.0, 1.0)))) {
        phase = clockTime.x;
    } else if (all(equal(color, vec3(0.0, 1.0, 0.0)))) {
        phase = clockTime.y;
    } else if (all(equal(color, vec3(1.0, 0.0, 0.0)))) {
        phase = clockTime.z;
    }

    float angle = -two_pi * phase;
    float c = cos(angle);
    float s = sin(angle);
    mat2 rotation = mat2(c, s, -s, c);

    vec3 transformedPosition = position;
    transformedPosition.xy = rotation * position.xy;
    transformedPosition.x = -transformedPosition.x;

    vertexColor = color;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix
                * vec4(transformedPosition, 1.0);
}
