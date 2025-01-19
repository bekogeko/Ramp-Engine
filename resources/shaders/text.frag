#version 330

out vec4 FragColor;
uniform vec4 uColor;

in vec2 TexCoord;
in vec4 TexDataCoord;
uniform sampler2D textureID;

void main() {

    float alpha = texture(textureID, mix(TexDataCoord.xy, TexDataCoord.zw, TexCoord)).r;
    FragColor = vec4(1, 1, 1, alpha) * uColor;
}