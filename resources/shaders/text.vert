#version 330

layout (location = 0) in vec2 aPos;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;


uniform vec2 uText;


void main() {
    gl_Position = uProjection * uModel * vec4((aPos), 0.0, 1.0);
}