#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

uniform vec3 lightPosition = vec3(10.0, 10.0, 10.0);

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition);

    float dotProduct = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = dotProduct * vec3(0.385, 0.647, 0.812);
    FragColor = vec4(diffuse, 1.0);
}
