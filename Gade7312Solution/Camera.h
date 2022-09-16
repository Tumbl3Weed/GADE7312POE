#pragma once

#include <iostream>
#include <vector>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h> 

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// The directions our camera can move
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_Location
{
	POSITION1,

};
// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 100.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
private:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	glm::vec3 Position1 = glm::vec3(67.0f, 300.5f, 169.9f);
	glm::vec3 Position2 = glm::vec3(-67.0f, 300.5f, 169.9f);
	glm::vec3 Position3 = glm::vec3(-67.0f, 300.5f, -169.9f);
	glm::vec3 centerWorld = glm::vec3(0.0f, 0.0f, 0.0f);

	GLint positionCounter = 1;
	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);

		// Recalculate the right and up vectors
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
		// Normalise the vectors because their length gets closer to 0 the more you look up and down 
		// which results in slower movement.
	}

public:
	//Constructor with Vectors
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	//Constructor with Scalars
	Camera(
		GLfloat posX, GLfloat posY, GLfloat posZ,
		GLfloat upX, GLfloat upY, GLfloat upZ,
		GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Getter for view matrix
	// Returns view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	glm::mat4 GetViewMatrixCentre()
	{
		return glm::lookAt(this->position, this->centerWorld + this->front, this->up);
	}



	int glfwGetKey(GLFWwindow* window, int key)	// Process keyboard input to detect which way the camera should move
	{

	}
	void ProcessKeyboard(Camera_Movement direction)
	{
		GLfloat velocity = this->movementSpeed;

		//if (direction == FORWARD)
		//{
		//	this->position += this->front * velocity;
		//}

		//if (direction == BACKWARD)
		//{
		//	this->position -= this->front * velocity;
		//}


		if (direction == LEFT)
		{
			positionCounter -= 1;

		}

		if (direction == RIGHT)
		{
			positionCounter += 1;

		}
		if (positionCounter >= 4)
			positionCounter = 1;
		if (positionCounter <= 0)
			positionCounter = 3;

		if (positionCounter == 1)
			this->position = Position1;
		if (positionCounter == 2)
			this->position = Position2;
		if (positionCounter == 3)
			this->position = Position3;



	}

	// Process Mouse input in x and y directions
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		//xOffset *= this->mouseSensitivity;
		//yOffset *= this->mouseSensitivity;

		//this->yaw += xOffset;
		//this->pitch += yOffset;

		//// Make sure that when pitch is out of bounds, screen doesn't get flipped
		//if (constrainPitch)
		//{
		//	if (this->pitch > 89.0f)
		//	{
		//		this->pitch = 89.0f;
		//	}

		//	if (this->pitch < -89.0f)
		//	{
		//		this->pitch = -89.0f;
		//	}
		//}

		//// Update front, right and up vectors using the updated Euler angles
		//this->updateCameraVectors();
	}

	// Process mouse scroll for zooming
	void ProcessMouseScroll(GLfloat yOffset)
	{
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		{
			this->zoom -= yOffset;
		}

		if (this->zoom <= 1.0f)
		{
			this->zoom = 1.0f;;
		}

		if (this->zoom >= 45.0f)
		{
			this->zoom = 45.0f;;
		}
	}

	// Getter for Zoom variable
	GLfloat GetZoom()
	{
		return this->zoom;
	}
};