#include <iostream>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//SOIL
#include "SOIL2.h"

// GLFW
#include <GLFW/glfw3.h>

// Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Create shader for Triangle vertices
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

// Create shader for Triangle colour & texture
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

// MAIN FUNCTION for MAIN GAME LOOP
int main()
{
	// Initialize GLFW
	glfwInit();

	// Set all required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create our Window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tester", nullptr, nullptr);

	// Actual screen resolution
	int screenW, screenH;

	// Get screen resolution
	glfwGetFramebufferSize(window, &screenW, &screenH);

	// Check if window was created successfully
	if (nullptr == window)
	{
		cout << "Failed to create window." << endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); // Exit

	// Enable GLEW
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (GLEW_OK != glewInit())
	{
		cout << "Failed to initialise GLEW." << endl;
		return EXIT_FAILURE;
	}

	// Setup OpenGL viewport
	// Define viewport dimensions
	glViewport(0, 0, screenW, screenH);

	/* BUILD & COMPILE TRIANGLE */

	// Build Shader (Vertex Shader)
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creating the variable
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile Shader (Vertex Shader)
	glCompileShader(vertexShader);

	// Create variables for error handling
	GLint success;
	GLchar infoLog[512];

	// Compile vertex shader & store status in success variable
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// Check if vertex compilation was successful
	if (!success)
	{
		// Store compilation errors in infoLog variable
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Build Shader (Fragment Shader)
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creating the variable
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile Shader (Fragment Shader)
	glCompileShader(fragmentShader);

	// Compile fragment shader & store status in success variable
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	// Check if fragment compilation was successful
	if (!success)
	{
		// Store compilation errors in infoLog variable
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Link Shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for Linking errors and save it success variable
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	// Check if shaders were successfully linked
	if (!success)
	{
		// Store linking errors in infoLog variable
		//glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Shaders linked successfully
	// Remove individual shaders because they are now linked in shaderProgram
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set vertex data for our Triangle corners
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // Left
		0.5f, -0.5f, 0.0f, // Right
		0.0f, 0.5f, 0.0f // Top
	};

	// Generate vertex object and vertex buffer arrays and save them into the VOA, VBA variables
	GLuint VOA, VBA;
	glGenVertexArrays(1, &VOA);
	glGenBuffers(1, &VBA);

	// Bind the vertex array object
	glBindVertexArray(VOA);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

	/* BUILD & COMPILE TRIANGLE */

	// GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Checks for events and calls corresponding response
		glfwPollEvents();

		//Render
		// Clear the colour buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* DRAW OUR TRIANGLE */
		glUseProgram(shaderProgram);
		glBindVertexArray(VOA);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); // Unbinding
		/* DRAW OUR TRIANGLE */

		// Draw the OpenGl window/viewport
		glfwSwapBuffers(window);
	}

	// Properly deallocate all resources
	glDeleteVertexArrays(1, &VOA);
	glDeleteBuffers(1, &VBA);

	// Terminate GLFW and clear any resources from GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}