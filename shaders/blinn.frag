#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

uniform mat4 viewMatrix = mat4(1.0);
uniform vec3 lightPosition = vec3(10.0, 10.0, 10.0);

uniform float shininess = 32.0;

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition);
    vec3 viewPos = vec3(inverse(viewMatrix)[3]);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 halfDir = normalize(lightDir + viewDir);

    float dotProduct = max(dot(lightDir, normal), 0.0);
    float spec = pow(max(dot(normal, halfDir), 0.0), shininess);

    vec3 ambient = vec3(0.1);
    vec3 diffuse = dotProduct * vec3(0.385, 0.647, 0.812);
    vec3 specular = 0.5 * spec * vec3(1.0);
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
