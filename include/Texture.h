#pragma once

#include <GL/glew.h>
#include <string>
#include <soil.h>
#include <iostream>

/*
����shader��������ͨ�ģ�����float�� int�ȣ�shader��ҪglUniform1i(location, value);
����sampler2D����������һ��Ҳ�Ǳ�Ҫ�ģ���ҪglUniform1i(location, value), �˴���value��ʾ����Ԫ��
��������Ԫ��Ӧ���������ݣ�����Ҫͨ��glActiveTexture(unit) &&glBindTexture()������Ч��
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
		// �ϲ����֮ǰҪ��glUniform1i��shader�Ĳ�������Ӧ������Ԫ
		// glUniform1i(glGetLocation(shaderid, "texture_param"), offset);
		// Texture.Bind(GL_TEXTURE0 + offset);
		//���shader��ֻ��һ��sample2D��������ֱ��Bind��GL_TEXTURE0������Ҫ�����glUniform1i������
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
		glGenerateMipmap(m_target);  // �Զ�����LOD����
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