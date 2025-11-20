#version 330 core

in vec3 frag_col;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	float ambientLight = 0.5f;
	vec3 ambient = ambientLight * lightColor;

	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(norm, lightDir), 0.0f);
	float diffuse = diffuseLight * lightColor;


	vec3 result = (ambient + diffuse) * frag_col;

	color = vec4(result, 1.0f);
}