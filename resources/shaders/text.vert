#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;


out vec2 TexCoord;


void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}