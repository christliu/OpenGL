#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <soil.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include"Shader.h"
#include"Camera.h"
#include "Texture.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0, 0, 10);

glm::vec3 lightpos = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 lightcolor = glm::vec3(1, 1, 1);

//GLfloat cameraSpeed = 5.0f;
GLboolean keys[1024];
GLfloat delta = 0.0f;
GLfloat lastframe = GLfloat(glfwGetTime());

GLfloat lastX = 400.0f, lastY = 300.0f;
GLfloat firstMouse = true;

Camera camera(cameraPos);


void doMovement();

void CreateSphere(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();
	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

	std::vector<GLfloat> sphereVertices;
	std::vector<GLuint> sphereIndices;
	CreateSphere(sphereVertices, sphereIndices);
	GLuint sphereVAO, sphereVBO, sphereEBO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	float stride = (3 + 2 + 3) * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glBindVertexArray(0);

	Shader shader("shader/13.1.PBRLighting.vs", "shader/13.1.PBRLighting.ps");


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLfloat start = (GLfloat)glfwGetTime();
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		// Render
		// Clear the colorbuffer

		delta = (GLfloat)glfwGetTime() - lastframe;
		lastframe = (GLfloat)glfwGetTime();

		doMovement();


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);


		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 perspective = glm::mat4(1.0f);
		perspective = glm::perspective(glm::radians(camera.Zoom), float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);

		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(perspective));
		glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "CamPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightPositions[0]"), lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightPositions[1]"), lightPositions[1].x, lightPositions[1].y, lightPositions[1].z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightPositions[2]"), lightPositions[2].x, lightPositions[2].y, lightPositions[2].z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightPositions[3]"), lightPositions[3].x, lightPositions[3].y, lightPositions[3].z);
		
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightColors[0]"), lightColors[0].x, lightColors[0].y, lightColors[0].z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightColors[1]"), lightColors[1].x, lightColors[1].y, lightColors[1].z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightColors[2]"), lightColors[2].x, lightColors[2].y, lightColors[2].z);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "lightColors[3]"), lightColors[3].x, lightColors[3].y, lightColors[3].z);

		glUniform1f(glGetUniformLocation(shader.m_shaderProgram, "roughness"), 0.5);
		glUniform1f(glGetUniformLocation(shader.m_shaderProgram, "ao"), 1.0);
		glUniform1f(glGetUniformLocation(shader.m_shaderProgram, "metallic"), 0.5);
		glUniform3f(glGetUniformLocation(shader.m_shaderProgram, "albedo"), 0.5f, 0.0f, 0.0f);

		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLE_STRIP, sphereIndices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);
	//glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos, lastY = ypos;
		firstMouse = false;
	}
	GLfloat deltax = xpos - lastX;
	GLfloat dletay = ypos - lastY;
	camera.ProcessMouseMovement(deltax, dletay);
	lastX = xpos;
	lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void doMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, delta);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, delta);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, delta);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, delta);
}

void CreateSphere(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	// vertices = {
	//	x, y, z, u, v, nx, ny, nz
	// }
	std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;

	const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y       * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y       * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    for (int i = 0; i < positions.size(); ++i)
    {
        vertices.push_back(positions[i].x);
        vertices.push_back(positions[i].y);
        vertices.push_back(positions[i].z);
        if (uv.size() > 0)
        {
            vertices.push_back(uv[i].x);
            vertices.push_back(uv[i].y);
        }
        if (normals.size() > 0)
        {
            vertices.push_back(normals[i].x);
            vertices.push_back(normals[i].y);
            vertices.push_back(normals[i].z);
        }
    }
}