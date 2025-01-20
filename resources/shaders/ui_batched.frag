#version 330

out vec4 FragColor;
in vec4 Color;

void main() {
    FragColor = vec4(Color.xyz, Color.a);
}