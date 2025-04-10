#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord; // (0,0), (0,1), (1,0), (1,1)


layout (location = 2) in vec2 cursorPos; // Instance data
layout (location = 3) in vec4 texData; // Instance data (min_s,min_t,max_s,max_t)
layout (location = 4) in vec2 charSize; // Instance data (min_s,min_t,max_s,max_t)

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;


out vec2 TexCoord;
out vec4 TexDataCoord;

void main() {

    vec4 pos = vec4(aPos * charSize, 0.0, 1.0);
    pos.xy += cursorPos; // Apply cursorPos to the position
    gl_Position = uView * uProjection * uModel * pos;

    TexCoord = aTexCoord;
    TexDataCoord = texData;
}