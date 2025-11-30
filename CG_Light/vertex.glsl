#version 330 core

layout(location = 0) in vec3 loc;
layout(location = 1) in vec3 col;
layout(location = 2) in vec3 norm;

out vec3 frag_col;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 Transform;

void main()
{
	gl_Position = Transform * vec4(loc, 1.0f);
	FragPos = vec3(model * vec4(loc, 1.0f));
	Normal = mat3(transpose(inverse(model))) * norm;

	frag_col = col;
}