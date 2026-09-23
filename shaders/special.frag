#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vec3(1.0, 1.0, 1.0) - vertexColor, 1.0);
}
