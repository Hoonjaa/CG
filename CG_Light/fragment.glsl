#version 330 core

in vec3 FragPos;
in vec3 frag_col;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	float ambientLight = 0.6f;
	vec3 ambient = ambientLight * lightColor;

	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0f);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 128;
	vec3 viewDir = normalize (viewPos - FragPos);
	vec3 reflectDir = reflect (-lightDir, normalVector);
	float specularLight = max (dot (viewDir, reflectDir), 0.0f);
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor;


	vec3 result = (ambient + diffuse + specular) * frag_col;

	color = vec4(result, 1.0f);
}