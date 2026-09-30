#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

uniform float angle = 0.5745329252;

void main() {
    vertexColor = color;
    // gl_Position
    //     = vec4(vec3(1, 1, -1) * 0.4 * position + vec3(-0.55, 0.0, 0.0), 1.0);
    //  gl_Position = vec4(position, 1.0);
    vec3 p = position;

    float x_ = cos(angle) * p.x + sin(angle) * p.z;
    float z_ = -sin(angle) * p.x + cos(angle) * p.z;

    p.x = x_;
    p.z = z_;

    gl_Position = vec4(vec3(1, 1, -1) * 0.4 * p, 1.0);
}
