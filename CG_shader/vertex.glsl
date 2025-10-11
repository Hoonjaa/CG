#version 330 core

layout(location = 0) in vec3 loc;
layout(location = 1) in vec3 col;
out vec3 frag_col;

void main()
{
	gl_Position =  vec4(loc, 1.0f);
	frag_col = col;
}