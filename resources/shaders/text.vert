#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 cursorPos; // Instance data

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;


out vec2 TexCoord;

void main() {

    vec4 pos = uModel * vec4(aPos, 0.0, 1.0);
    pos.xy += cursorPos; // Apply cursorPos to the position
    gl_Position = uProjection * pos;

    //    gl_Position = uProjection * uModel * (vec4(aPos, 0.0, 1.0));
    TexCoord = aTexCoord;
}