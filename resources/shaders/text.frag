#version 330

out vec4 FragColor;
uniform vec4 uColor;

in vec2 TexCoord;
uniform sampler2D textureID;

void main() {
    //    FragColor = uTextColor * uColor;
    FragColor = vec4(1, 1, 1, texture(textureID, TexCoord).r) * uColor;
}