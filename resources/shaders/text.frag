#version 330

out vec4 FragColor;
uniform vec4 uColor;

in vec2 TexCoord;
uniform sampler2D textureID;

void main() {
    //    FragColor = uTextColor * uColor;
    FragColor = vec4(texture(textureID, TexCoord).r, 0, 0, texture(textureID, TexCoord).r + 0) * uColor;

}