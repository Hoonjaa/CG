#version 330 core

in vec2 loc;

void main()
{
	gl_Position =  vec4(loc.x, loc.y, 0.0f, 1.0f);
}