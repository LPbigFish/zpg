#version 330 core

layout(location = 0) in vec3 position;

out vec3 textureDirection;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    textureDirection = position;

    mat4 view = mat4(mat3(viewMatrix));

    gl_Position = projectionMatrix * view * vec4(position, 1.0);
}
