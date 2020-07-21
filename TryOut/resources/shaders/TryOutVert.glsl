#version 330 core
layout (location = 0) in vec2 v_Vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 v_TexCoords;
layout (location = 2) in vec2 v_Offset;
out vec2 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_Model * vec4(v_Vertex + v_Offset, 0.0, 1.0);
    TexCoords = v_TexCoords;
} 