#pragma once
#include<glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat MOVESPEED = 5.0f;
const GLfloat SENSITIVE = 0.01f;
const GLfloat ZOOM = 45.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	glm::vec3 Front;

	glm::vec3 Position;
	glm::vec3 Right;
	glm::vec3 Up;

	glm::vec3 WorldUp;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat MoveSpeed;
	GLfloat MouseSensitive;
	GLfloat Zoom;

	Camera(
		glm::vec3 position = glm::vec3(0, 0, 0), 
		glm::vec3 up = glm::vec3(0, 1, 0), 
		GLfloat yaw = YAW, 
		GLfloat pitch = PITCH):Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(MOVESPEED), MouseSensitive(SENSITIVE), Zoom(ZOOM)
	{
		this->yaw = yaw;
		this->pitch = pitch;
		Position = position;
		WorldUp = up;
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MoveSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitive;
		yoffset *= this->MouseSensitive;

		this->yaw += xoffset;
		this->pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->pitch > 89.0f)
				this->pitch = 89.0f;
			if (this->pitch < -89.0f)
				this->pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		UpdateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
		/*glm::vec3 direction = glm::normalize(this->Front);
		GLfloat speed = 0.1f;
		this->Position += yoffset * speed * direction;*/

	}

private:
	void UpdateCameraVectors()
	{
		glm::vec3 front = glm::vec3(1.0f);

		front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
		front.y = glm::sin(glm::radians(pitch));
		front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));

		//std::cout << "pitch " << pitch << std::endl;
		//std::cout << "yaw " << yaw << std::endl;
		//std::cout << "Front " << Front.x << " " << Front.y << " " << Front.z << std::endl;
		//std::cout << "Right " << Right.x << " " << Right.y << " " << Right.z << std::endl;
		//std::cout << " Up " << Up.x << " " << Up.y << " " << Up.z << std::endl;
	}

};