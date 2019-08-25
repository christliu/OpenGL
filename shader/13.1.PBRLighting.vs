#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	
	TexCoords = coord;
}