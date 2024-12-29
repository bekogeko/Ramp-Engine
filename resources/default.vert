#version 330

layout (location = 0) in vec2 aPos;

uniform vec2 uPosition;


void main() {
    gl_Position = vec4(aPos + uPosition, 0.0, 1.0);
}