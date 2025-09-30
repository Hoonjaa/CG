#version 330 core

in vec2 spot;

void main()
{
	gl_Position =  vec4(spot.x, spot.y, 0.0f, 1.0f);
}