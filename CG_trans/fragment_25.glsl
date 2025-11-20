#version 330 core

in vec3 frag_col;
out vec4 color;

void main()
{
	float ambientLight = 0.5f;
	vec3 ambient = ambientLight * frag_col;

	vec3 result = ambient;

	color = vec4(result, 1.0f);
}