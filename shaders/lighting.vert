#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 worldPosition;

out vec3 worldNormal;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

uniform mat3 normalMatrix = mat3(1.0);
uniform bool wrongNormals = false;

void main() {
    vertexColor = color;
    vertexNormal = normal;

    if (wrongNormals) {
        worldNormal = normalize(mat3(modelMatrix) * normal);
    } else {
        worldNormal = normalize(normalMatrix * normal);
    }

    vec4 worldPos = modelMatrix * vec4(position, 1.0);

    worldPosition = worldPos.xyz;

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
