#version 330

out vec4 FragColor;
uniform vec4 uColor;
//uniform vec4 uTextColor;

void main() {
    //    FragColor = uTextColor * uColor;
    FragColor = uColor;

}