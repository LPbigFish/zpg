#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

void main() {
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    FragColor = vec4(ambient * vec3(0.385, 0.647, 0.812), 1.0);
}
