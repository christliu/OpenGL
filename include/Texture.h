#pragma once

#include <GL/glew.h>
#include <string>
#include <soil.h>
#include <iostream>

/*
对于shader参数，普通的，例如float， int等，shader需要glUniform1i(location, value);
对于sampler2D参数，上面一部也是必要的，需要glUniform1i(location, value), 此处的value表示纹理单元。
具体纹理单元对应的纹理数据，还需要通过glActiveTexture(unit) &&glBindTexture()才能生效。
*/
class OpenGLTexture
{
public:
	OpenGLTexture(GLenum texture_target, const std::string path)
	{
		m_path = path;
		m_target = texture_target;
		Load(m_path.c_str());
	}

	void Bind(GLenum texture_unit)
	{
		// 上层调用之前要先glUniform1i绑定shader的参数到对应的纹理单元
		// glUniform1i(glGetLocation(shaderid, "texture_param"), offset);
		// Texture.Bind(GL_TEXTURE0 + offset);
		//如果shader中只有一个sample2D参数，则直接Bind到GL_TEXTURE0，不需要额外的glUniform1i操作。
		glActiveTexture(texture_unit);
		glBindTexture(m_target, m_textureID);
	}

private:
	void Load(const GLchar* path)
	{
		GLint width, height;
		unsigned char* img = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
		if (!img)
		{
			std::cout << "Error Imag Path " << path << std::endl;
			return;
		}
		glGenTextures(1, &m_textureID);
		glBindTexture(m_target, m_textureID);
		glTexImage2D(m_target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(m_target);  // 自动生成LOD纹理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
   	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	// set texture filtering parameters
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SOIL_free_image_data(img);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	std::string m_path;
	GLenum m_target;
	GLuint m_textureID;
};