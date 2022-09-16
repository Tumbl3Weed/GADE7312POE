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

// Enum to handle change in camera position
enum Camera_Movement
{
	LEFT,
	RIGHT
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

	// Camera vector3 locations to cycle to
	glm::vec3 Position1 = glm::vec3(67.0f, 200.5f, 169.9f);
	glm::vec3 Position2 = glm::vec3(-67.0f, 200.5f, 160.0f);
	glm::vec3 Position3 = glm::vec3(-80.0f, 200.5f, -80.0f);

	//world center to look at
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
		// Calculate the new Front vector relative to the camera
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);

		// Recalculate the right and up vectors relative to the camera
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
		// Normalise the vectors because their length gets closer to 0 the more you look up and down 
		
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
	// Returns view matrix calculated using Euler Angles and the 
	// LookAt Matrix at the center of the world where the chessboard is
	glm::mat4 GetViewMatrixCentre()
	{
		this->updateCameraVectors();
		return glm::lookAt(this->position, this->centerWorld + this->front, this->up);

	}


	void ProcessKeyboard(Camera_Movement direction)// Process keyboard and move camera between 3 static positions
	{

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

	GLfloat GetZoom() {
		return this->zoom;
	}

};