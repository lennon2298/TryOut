#version 330 core
layout (location = 0) in vec2 v_Vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 v_TexCoords;
layout (location = 2) in vec2 v_Offset;
out vec2 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_Model;

void main()
{
    vec4 pos = u_Model * vec4(v_Vertex, 0.0, 1.0);
    pos.xy += v_Offset;
		
    gl_Position = u_Projection * pos;
    TexCoords = v_TexCoords;
} 