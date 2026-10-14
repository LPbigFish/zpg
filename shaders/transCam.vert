#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 modelMatrix = mat4(1.0);

uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main() {
    vertexColor = color;
    vertexNormal = normal;

    gl_Position
        = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
