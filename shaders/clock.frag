#version 330 core

flat in vec3 vertexColor;

out vec4 FragColor;

void main() {
    if (all(equal(vertexColor, vec3(0.0, 0.0, 1.0)))) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else if (all(equal(vertexColor, vec3(0.0, 1.0, 0.0)))) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else if (all(equal(vertexColor, vec3(1.0, 1.0, 0.0)))) {
        FragColor = vec4(0.0, 0.643, 0.60, 1.0);
    } else {
        FragColor = vec4(vertexColor, 1.0);
    }
}
