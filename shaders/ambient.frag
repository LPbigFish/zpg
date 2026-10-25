#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 FragColor;

uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

uniform vec3 color = -vec3(1.0);

void main() {
    vec3 u_color = (all(equal(color, -vec3(1.0))) ? vertexColor : color);

    FragColor = vec4(ambient * u_color, 1.0);
}
