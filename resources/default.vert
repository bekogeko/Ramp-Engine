#version 330

layout (location = 0) in vec2 aPos;

uniform vec2 uPosition;

uniform mat4 uProjection;



void main() {

    vec4 transformedPos = vec4(aPos + uPosition, 0.0, 1.0);
    gl_Position = uProjection * transformedPos;
}