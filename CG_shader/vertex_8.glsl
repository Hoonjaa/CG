#version 330 core

uniform vec4 u_position;

void main()
{
	gl_Position =  u_position;
}