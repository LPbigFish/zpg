#version 330 core

in vec3 worldNormal;

out vec4 FragColor;

void main() {
    FragColor = vec4(normalize(worldNormal) * 0.5 + 0.5, 1.0);
}
