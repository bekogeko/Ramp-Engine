#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aCursorPos;
layout (location = 2) in vec2 aSize;
layout (location = 3) in vec4 aColor;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec4 Color;

void main() {

    vec2 pos = (aPos + aCursorPos) * aSize;
    gl_Position = uProjection * vec4(pos, 0.0, 1.0);
    Color = aColor;
}