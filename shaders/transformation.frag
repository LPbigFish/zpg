#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

uniform vec3 fragmentColor = vec3(1, 1, 0);

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
