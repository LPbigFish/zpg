#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

uniform mat4 viewMatrix = mat4(1.0);
uniform vec3 lightPosition = vec3(10.0, 10.0, 10.0);

uniform float shininess = 32.0;

uniform vec3 color = -vec3(1.0);

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition);
    vec3 viewPos = vec3(inverse(viewMatrix)[3]);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 reflectionDir = reflect(-lightDir, normal);

    vec3 u_color = (all(equal(color, -vec3(1.0))) ? vertexColor : color);

    float dotProduct = max(dot(lightDir, normal), 0.0);
    float spec = dotProduct > 0.0
                   ? pow(max(dot(viewDir, reflectionDir), 0.0), shininess)
                   : 0.0;

    vec3 ambient = 0.1 * u_color;
    vec3 diffuse = dotProduct * u_color;
    vec3 specular
        = 0.5 * spec * vec3(1.0); // specularStrength * spec * light_color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
