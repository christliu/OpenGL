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

#include"Shader.h"
#include"Camera.h"
#include "Texture.h"
//#include "Model.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

glm::vec3 cameraPos = glm::vec3(0, 0, 3);

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

//GLfloat cameraSpeed = 5.0f;
GLboolean keys[1024];
GLfloat delta = 0.0f;
GLfloat lastframe = GLfloat(glfwGetTime());

GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
GLfloat firstMouse = true;

Camera camera(cameraPos);

void doMovement();
void RenderScene(Shader &shader);

GLuint FloorVAO, CubeVAO, QuadVAO;

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
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

	float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    float cubeVertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    GLuint FloorVBO;
	glGenVertexArrays(1, &FloorVAO);
	glGenBuffers(1, &FloorVBO);
	glBindVertexArray(FloorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, FloorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0);

    GLuint CubeVBO;
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);
	glBindVertexArray(CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0);

	GLuint QuadVBO;
	glGenVertexArrays(1, &QuadVAO);
	glGenBuffers(1, &QuadVBO);
	glBindVertexArray(QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0);


	Shader shader("shader/11.1.ShadowMap.vs", "shader/11.1.ShadowMap.ps");
	Shader quadShader("shader/11.1.ShadowDepth.vs", "shader/11.1.ShadowDepth.ps");
	Shader sceneShader("shader/11.2.ShadowMap.vs", "shader/11.2.ShadowMap.ps");

	OpenGLTexture wood(GL_TEXTURE_2D, "res/textures/wood.png");

	GLuint depthMap;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	GLfloat start = (GLfloat)glfwGetTime();
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		// Render
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		delta = (GLfloat)glfwGetTime() - lastframe;
		lastframe = (GLfloat)glfwGetTime();

		doMovement();
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);
		shader.Use();
		wood.Bind(GL_TEXTURE0);

		glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 17.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //lightProjection = glm::perspective(glm::radians(90.0f), float(WIDTH) / float(HEIGHT), 0.1f, 10.0f);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
		glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderScene(shader);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(sceneShader.m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(sceneShader.m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(sceneShader.m_shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniform3f(glGetUniformLocation(sceneShader.m_shaderProgram, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(sceneShader.m_shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		glUniform1i(glGetUniformLocation(sceneShader.m_shaderProgram, "diffuseTexture"), 0);
		glUniform1i(glGetUniformLocation(sceneShader.m_shaderProgram, "shadowMap"), 1);


		wood.Bind(GL_TEXTURE0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		RenderScene(sceneShader);

		 //quadShader.Use();
		 //glActiveTexture(GL_TEXTURE0);
   //      glBindTexture(GL_TEXTURE_2D, depthMap);
   //      glBindVertexArray(QuadVAO);
   //      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   //      glBindVertexArray(0);
   //      glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	// Properly de-allocate all resources once they've outlived their purpose
	//glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


void RenderScene(Shader &shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(FloorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindVertexArray(CubeVAO);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::scale(model, glm::vec3(0.25f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
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
	if(keys[GLFW_KEY_E])
		camera.ProcessKeyboard(UP, delta);
	if(keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(DOWN, delta);
}