# Texture

Texture在shader中以uniform sample2D的参数格式出现，以下是贴图shader示例

~~~cpp
#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
   color = vec4(texture(texture_diffuse1, TexCoords));
}
~~~

## 纹理单元

* OpenGL保证有16个纹理单元。

* GL_TEXTURE0 到 GL_TEXTURE15

* 使用的时候用GL_TEXTURE0 + 8 可以表示 GL_TEXTURE8

* 程序往对应的纹理单元绑定纹理数据的示例代码为：

  ~~~ cpp
  glActiveTexture(GL_TEXTURE0 + offset);  // 如果offset=1，则往 GL_TEXTURE1绑定数据
  glBindTexture(GL_TEXTURE_2D, texture1);
  ~~~

* 如果希望shader中uniform参数对应纹理单元，需要调用：

  ~~~
  glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), offset);
  ~~~

## 通用的textuer类

~~~cpp
#pragma once

#include <GL/glew.h>
#include <string>
#include <soil.h>
#include <iostream>

/*
对于shader参数，普通的，例如float， int等，shader需要glUniform1i(location, value);
对于sampler2D参数，上面一部也是必要的，需要glUniform1i(location, value), 此处的value表示纹理单元。
具体纹理单元对应的纹理数据，还需要通过glActiveTexture(unit) && glBindTexture()才能生效。
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
		SOIL_free_image_data(img);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	std::string m_path;
	GLenum m_target;
	GLuint m_textureID;
};
~~~

##  texture类使用方法

~~~
Texture texture1(GL_TEXTURE_2D, "../../path/to/texture1");
Texture texture2(GL_TEXTURE_2D, "../../path/to/texture2");
...
shader.Use();

glUniform1i(glGetUniformLocation(shader.m_shaderProgram, "material.diffuse"), 0);
								// 指定material.diffuse占用纹理单元0
glUniform1i(glGetUniformLocation(shader.m_shaderProgram, "material.specular"), 1);
texture1.Bind(GL_TEXTURE0); 	// 纹理单元0绑定texture1的数据。
texture2.Bind(GL_TEXTURE1);

ourModel.Draw(shader);

~~~

