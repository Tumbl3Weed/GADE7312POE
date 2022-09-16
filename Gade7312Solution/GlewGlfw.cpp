#include <iostream>
#include <vector>
using namespace std;

// GLEW library
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW library
#include <GLFW/glfw3.h>

// SOIL2 library
#include "SOIL2/SOIL2.h"

// GLM library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//local header files
#include "Shader.h"
#include "Camera.h"

// Declare Callback Methods
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);

// Settings of the window height and width
const GLint WIDTH = 800;
const GLint HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera initialized in the 3D space at a point
Camera camera(glm::vec3(67.0f, 300.5f, 169.9f), glm::vec3(0.0f, 1.0f, 0.0f), -128.1f, -42.4f);
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;

bool keys[1024]; // Array of 1024 different types of keys, used for checks against their usage
bool firstMouse = true; // We only handling one type of mouse input, thus its true

// Variables used to track time for calculations
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Initialize opengl framework
	glfwInit();

	// Setting options of the window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Creating Window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tester", nullptr, nullptr);

	// Get screen resolution
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Check if window was created successfully
	if (window == nullptr)
	{
		//if the window wasn't created, write console line and exit
		cout << "Failed to create window." << endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	//Direct opengl to the window we using
	glfwMakeContextCurrent(window); 

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);

	// Center cursor to window and Hide the cursor for a more immersive experience
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable GLEW
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (GLEW_OK != glewInit())
	{
		cout << "Failed to initialize GLEW" << endl;
		return EXIT_FAILURE;
	}

	// Setup OpenGL viewport
	// Define viewport dimensions within the scope if the system
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 

	// Enable depth in the project
	glEnable(GL_DEPTH_TEST);

	//Build & Compile Shader Program
	Shader ourShader("core.vs", "core.frag");

	// Create and load texture using our image path
	int width, height, numberOfChannels;

	unsigned char* imageData = SOIL_load_image("res/images/bitmapheightmap.png", &width, &height, &numberOfChannels, 0);

	if (imageData)
	{
		cout << "Loaded heightmap of size " << height << " x " << width << endl;
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	vector<GLfloat> vertices;
	GLfloat yScale = 50.0f / 500.0f; //normalize the height map data and scale it to the desired height
	int rez = 1;
	GLuint bytePerPixel = numberOfChannels;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char* pixelOffset = imageData + (j + width * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-height / 2.0f + height * i / (float)height); // vertex x pos
			vertices.push_back((float)y * yScale-2.0f); // vertex y pos
			vertices.push_back(-width / 2.0f + width * j / (float)width); // vertex z pos
		}
	}
	cout << "Loaded " << vertices.size() / 3 << " vertices" << endl;
	SOIL_free_image_data(imageData);

	vector<GLuint> indices;
	for (int i = 0; i < height - 1; i += rez)
	{
		for (int j = 0; j < width; j += rez)
		{
			for (int k = 0; k < 2; k++)
			{
				indices.push_back(j + width * (i + k * rez));
			}
		}
	}
	cout << "Loaded " << indices.size() << " indices" << endl;

	const int numStrips = (height - 1) / rez;
	const int numTrisPerStrip = (width / rez) * 2 - 2;
	cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << endl;
	cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << endl;

	// Generate the vertex arrays, vertex buffers and index buffers and save them into variables
	unsigned int VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	// GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cout << 1.0f / deltaTime << " FPS)" << endl; //Check FPS

		// Checks for events and calls corresponding response
		glfwPollEvents();

		// Handle the input

		// Render
		// Clear the colour buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate Shader
		ourShader.Use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100000.0f);
		glm::mat4 view = camera.GetViewMatrixCentre();
		GLint projLoc = glGetUniformLocation(ourShader.Program, "m4Projection");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "m4View");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "m4Model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Draw container
		glBindVertexArray(VAO);

		for (int strip = 0; strip < numStrips; strip++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, // primitive type
				numTrisPerStrip + 2, // number of indices to render
				GL_UNSIGNED_INT, // index data type
				(void*)(sizeof(GLuint) * (numTrisPerStrip + 2) * strip)); // offset to starting index
		}

		glBindVertexArray(0); // Unbinding

		// Draw the OpenGl window/viewport
		glfwSwapBuffers(window);
	}

	// Properly deallocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	// Terminate GLFW and clear any resources from GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;

		}
		else if (action == GLFW_RELEASE)
		{
			if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
			{
				camera.ProcessKeyboard(LEFT);
			}

			if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
			{
				camera.ProcessKeyboard(RIGHT);
			}
			keys[key] = false;
		}
	}
}