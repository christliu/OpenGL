#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"


Shader::Shader(const GLchar* vs, const GLchar* ps, const GLchar* gs)
{
	std::string vscode, pscode, gscode;

	std::ifstream vShaderFile, pShaderFile, gShaderFile;
	vShaderFile.exceptions(std::ifstream::badbit);
	pShaderFile.exceptions(std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		vShaderFile.open(vs);
		pShaderFile.open(ps);

		std::stringstream vss, pss, gss;
		vss << vShaderFile.rdbuf();
		pss << pShaderFile.rdbuf();

		vShaderFile.close();
		pShaderFile.close();

		vscode = vss.str();
		pscode = pss.str();

		if(gs)
		{
			gShaderFile.open(gs);
			gss << gShaderFile.rdbuf();
			gShaderFile.close();
			gscode = gss.str();
		}
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "READ SHADER FILE ERROR" << std::endl;
	}
	const GLchar* vShaderCode = vscode.c_str();
	const GLchar* pShaderCode = pscode.c_str();

	GLuint vertexShader, fragmentShader;
	GLuint geometryShader;

	GLint success;
	GLchar infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &pShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	if(gs)
	{
		const GLchar* gShaderCode = gscode.c_str();
		//GLuint geometryShader;
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &gShaderCode, NULL);
		glCompileShader(geometryShader);
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cout << "ERROR: Compile Geometry Shader\n" << infoLog << std::endl;
		}
	}

	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	if(gs != NULL)
	{
		glAttachShader(m_shaderProgram, geometryShader);
	}
	glLinkProgram(m_shaderProgram);
	// Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if(gs!=NULL)
	{
		glDeleteShader(geometryShader);
	}
	std::cout << "build shader success" << std::endl;
}

void Shader::Use()
{
	glUseProgram(m_shaderProgram);
}