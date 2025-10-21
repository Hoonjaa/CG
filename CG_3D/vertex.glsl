#version 330 core

layout(location = 0) in vec3 loc;
layout(location = 1) in vec3 col;
out vec3 frag_col;

uniform mat4 modelTransform;
uniform mat4 RTransform;

void main()
{
	gl_Position = modelTransform * RTransform * vec4(loc, 1.0f);
	frag_col = col;
}