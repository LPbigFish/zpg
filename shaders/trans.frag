#version 330 core

in vec3 vertexColor;

in vec3 vertexNormal;

out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
