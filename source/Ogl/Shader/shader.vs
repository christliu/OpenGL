#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 Normal;
out vec3 FragPos;

void main()
{
	FragPos = vec3(model * vec4(position, 1.0f));
    gl_Position = perspective * view * vec4(FragPos, 1.0f);
	
	Normal = mat3(transpose(inverse(model))) * normal;
}