#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

uniform vec3 lightPosition = vec3(10.0, 10.0, 10.0);

uniform vec3 color = -vec3(1.0);

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition);

    vec3 u_color = (all(equal(color, -vec3(1.0))) ? vertexColor : color);

    float dotProduct = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = dotProduct * u_color;
    FragColor = vec4(diffuse, 1.0);
}
