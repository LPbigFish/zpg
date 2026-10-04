#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

uniform vec3 offset = vec3(0.0, 0.0, 0.0);
uniform vec3 scale = vec3(1.0, 1.0, 1.0);

uniform float angle = 0.5745329252;

void main() {
    vertexColor = color;

    vec3 p = position * scale;
    
    vec3 rotatedPosition = vec3(
        p.x * cos(angle) - p.z * sin(angle),
        p.y,
        p.x * sin(angle) + p.z * cos(angle)
    );

    vec3 transformedPosition = rotatedPosition + offset;

    gl_Position = vec4(transformedPosition.xy, -transformedPosition.z, 1.0);
}
