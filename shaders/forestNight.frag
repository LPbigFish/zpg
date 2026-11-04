#version 330 core

in vec3 vertexColor;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

uniform mat4 viewMatrix = mat4(1.0);

uniform vec3 lightPosition = vec3(0.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 0.5);
uniform float intensity = 2.2;

uniform float kc = 1.0;
uniform float kl = 0.35;
uniform float kq = 0.10;

uniform float ambientStrength = 0.03;
uniform float shininess = 8.0;

uniform vec3 color = -vec3(1.0);

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition);
    vec3 viewPos = vec3(inverse(viewMatrix)[3]);
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 halfDir = normalize(lightDir + viewDir);

    vec3 u_color = (all(equal(color, -vec3(1.0))) ? vertexColor : color);

    float d = length(lightPosition - worldPosition);
    float attenuation = 1.0 / (kc + kl * d + kq * d * d);

    float diff = max(dot(lightDir, normal), 0.0);
    float spec
        = diff > 0.0 ? pow(max(dot(normal, halfDir), 0.0), shininess) : 0.0;

    vec3 ambient = ambientStrength * u_color;
    vec3 diffuse = diff * u_color;
    vec3 specular = 0.3 * spec * vec3(1.0);

    FragColor = vec4(
        ambient + lightColor * intensity * attenuation * (diffuse + specular),
        1.0
    );
}
