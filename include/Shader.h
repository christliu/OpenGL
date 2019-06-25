#pragma once
#include <GL/glew.h>

class Shader
{
public:
	Shader(const GLchar* vspath, const GLchar* pspath, const GLchar* gs=NULL);

	void Use();

	GLuint m_shaderProgram;
};