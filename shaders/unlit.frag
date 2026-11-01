#version 330 core

in vec3 vertexColor;

out vec4 FragColor;

// Task 3: the firefly itself, unlit so it glows against the dark forest.
uniform vec3 fireflyColor = vec3(1.0, 0.95, 0.4);

void main() {
    FragColor = vec4(fireflyColor, 1.0);
}
