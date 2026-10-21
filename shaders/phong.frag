#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

void main() {
    vec3 lightPosition = vec3(10.0, 10.0, 10.0);
    float dotProduct = max(
        dot(normalize(lightPosition - worldPosition), normalize(worldNormal)),
        0.0
    );
    vec4 diffuse = dotProduct * vec4(0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    FragColor = ambient + diffuse;
}
