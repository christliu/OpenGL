#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <string>
#include <vector>
#include <Shader.h>
#include <sstream>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

void PrintVertex(Vertex v)
{
	std::cout << "Positon (" << v.Position.x << " ," << v.Position.y << " ," << v.Position.z << ") " ;
	std::cout << "Normal (" << v.Normal.x << " ," << v.Normal.y << " ," << v.Normal.z << ") ";
	std::cout << "TexCoords (" << v.TexCoords.x << ", " << v.TexCoords.y << std::endl;
}

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setupMesh();
	}

	void Draw(Shader shader);

	void Brief()
	{
		std::cout << "Vertices Count " << vertices.size() << " Indices Count " << indices.size() << std::endl;
		GLuint VertexBriefCount = 10, IndexBriefCount = 10;
		for (int i = vertices.size() - 1; i >= vertices.size() - VertexBriefCount; i--)
		{
			std::cout << "Vertex " << i << std::endl;
			PrintVertex(vertices[i]);
		}
		std::cout << "Indices";
		for (int i = 0; i < IndexBriefCount; i++)
		{
			std::cout << " " << indices[i];
		}
		std::cout << std::endl;
	}

	GLuint VAO;

private:
	GLuint VBO, EBO;
	void setupMesh();
};

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			ss << diffuseNr++;
		}
		else if (name == "texture_specular")
		{
			ss << specularNr++;
		}
		number = ss.str();
		GLint location = glGetUniformLocation(shader.m_shaderProgram, (name + number).c_str());
		GLint locComp = glGetUniformLocation(shader.m_shaderProgram, "texture_diffuse1");
		if (location != -1)
		{
			glUniform1i(location, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);

}
