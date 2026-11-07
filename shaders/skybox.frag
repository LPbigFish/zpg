#version 330 core

in vec3 textureDirection;
out vec4 fragColor;

uniform samplerCube skyboxTexture;

void main() {
    fragColor = texture(skyboxTexture, textureDirection);
}
