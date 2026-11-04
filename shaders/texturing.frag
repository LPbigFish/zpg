#version 330 core

in vec2 vertexUV;

out vec4 FragColor;

uniform sampler2D textureUnitID;

void main() {
    FragColor = texture(textureUnitID, vertexUV);
}
