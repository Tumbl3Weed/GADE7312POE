// *** CREATE 3D CHESS BOARD ON TERRAIN *** //
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL2
#include "SOIL2/SOIL2.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

// Declare Callback Methods
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ProcessInput(GLFWwindow* window);

// Settings
const GLint WIDTH = 800;
const GLint HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera - starting point
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -128.1f, -42.4f);
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024]; // Array of 1024 different types of keys
bool firstMouse = true; // Only handling one type of mouse, thus true

// Timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

	// Get screen resolution
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Check if window was created successfully
	if (window == nullptr)
	{
		cout << "Failed to create window." << endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); // Exit

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

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
	// Define viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //***

	// Enable depth in the project
	glEnable(GL_DEPTH_TEST);

	// *** CODE FOR CHESS BOARD *** //
	//Build & Compile Shader Program
	Shader ourShaderBoard("coreBoard.vs", "coreBoard.frag");

	// Set vertex data for our cube
	GLfloat cubeVertices[] =
	{
		//Positions				//Texture Coords
		/// Front Triangles/ 2 triangle make cube face
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	    1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f,
		//2nd triangle vertices
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		/// Right Triangles
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
		0.5f, 0.5f,  0.0f,		1.0f, 1.0f,

		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		/// Back Triangles
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		1.0f, 1.0f,

		-0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		/// Left Triangles
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	    1.0f, 1.0f,

		-0.5f, -0.5f, 0.0f,	    1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		/// Top Triangles
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,

		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		/// Bottom Triangles
		-0.5f, -0.5f, 0.0f,	    0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,

		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,	    0.0f, 0.0f
	};


	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_Board, VOA_Board;
	glGenVertexArrays(1, &VOA_Board);
	glGenBuffers(1, &VBA_Board);

	// Bind the vertex array object
	glBindVertexArray(VOA_Board);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_Board);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);//Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

	// Create and load texture
	GLuint textureWhite, textureBlack, textureBoarder;
	int widthB, heightB;
	glGenTextures(1, &textureWhite);
	glBindTexture(GL_TEXTURE_2D, textureWhite);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* imageWhite = SOIL_load_image("res/images/white.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageWhite);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageWhite);
	glBindTexture(GL_TEXTURE_2D, 0);


	//starting new texture
	glGenTextures(1, &textureBlack);
	glBindTexture(GL_TEXTURE_2D, textureBlack);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* imageBlack = SOIL_load_image("res/images/black.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBlack);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageBlack);
	glBindTexture(GL_TEXTURE_2D, 0);


	//start of boarder texture
	glGenTextures(1, &textureBoarder);
	glBindTexture(GL_TEXTURE_2D, textureBoarder);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* imageBoarder = SOIL_load_image("res/images/boarder.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBoarder);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(imageBoarder);
	glBindTexture(GL_TEXTURE_2D, 0);
	// *** CODE FOR CHESS BOARD *** //

	// Positions of different cubes ***

	vector<glm::vec3> cubePosList;
	vector<glm::vec3> boarderPosList;
	float randomHeight;
	for (size_t x = 0; x < 8; x++)
	{
		for (size_t z = 0; z < 8; z++)
		{
			randomHeight = 0;
			randomHeight += (float)((float)rand() / RAND_MAX);
			randomHeight /= 2;
			randomHeight -= (float)0.25;

			cubePosList.push_back(glm::vec3((float)x, randomHeight, (float)z));
			float tempX = x;
			float tempZ = z;
			if (z == 0)
				tempZ -= 1;
			if (z == 7)
				tempZ += 1;
			if (x == 0)
				tempX -= 1;
			if (x == 7)
				tempX += 1;
			if (tempZ != z || tempX != x) {
				boarderPosList.push_back(glm::vec3(tempX, 0.0f, tempZ));
			}
			if (x == 0 && z == 0) {
				boarderPosList.push_back(glm::vec3(tempX, 0.0f, tempZ + 1));
				boarderPosList.push_back(glm::vec3(tempX + 1, 0.0f, tempZ));
			}
			if (x == 7 && z == 0) {
				boarderPosList.push_back(glm::vec3(tempX, 0.0f, tempZ + 1));
				boarderPosList.push_back(glm::vec3(tempX - 1, 0.0f, tempZ));
			}
			if (x == 0 && z == 7) {
				boarderPosList.push_back(glm::vec3(tempX, 0.0f, tempZ - 1));
				boarderPosList.push_back(glm::vec3(tempX + 1, 0.0f, tempZ));
			}
			if (x == 7 && z == 7) {
				boarderPosList.push_back(glm::vec3(tempX, 0.0f, tempZ - 1));
				boarderPosList.push_back(glm::vec3(tempX - 1, 0.0f, tempZ));
			}
		}
	}

	//Build & Compile Shader Program
	Shader ourShader("core.vs", "core.frag");

	// Create and load texture replace it with your own image path.
	int width, height, nrChannels;

	unsigned char* data = SOIL_load_image("res/images/bitmapheightmap.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		cout << "Loaded heightmap of size " << height << " x " << width << endl;
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	vector<GLfloat> vertices;
	GLfloat yScale = 25.0f / 150.0f; //normalize the height map data and scale it to the desired height
	GLfloat yShift = 16.0f; // translate the elevations to our final desired range
	int rez = 1;
	GLuint bytePerPixel = nrChannels;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-height / 2.0f + height * i / (float)height); // vx
			vertices.push_back((int)y * yScale - 10); // vy
			vertices.push_back(-width / 2.0f + width * j / (float)width); // vz
		}
	}
	cout << "Loaded " << vertices.size() / 3 << " vertices" << endl;
	SOIL_free_image_data(data);

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
		//cout << deltaTime << "ms (" << 1.0f / deltaTime << " FPS)" << endl; //Check FPS

		// Checks for events and calls corresponding response
		glfwPollEvents();

		// Handle the input
		ProcessInput(window);

		// Render
		// Clear the colour buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// *** CODE FOR CHESS BOARD *** //
		ourShaderBoard.Use();

		// Create Projection Matrix *** (moved into while loop in order to update zoom)
		glm::mat4 projection_Board(1.0f);
		//Perspective view ***
		projection_Board = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

		// Create camera transformation ***
		glm::mat4 view_Board(1.0f);
		view_Board = camera.GetViewMatrix();

		// Get the uniform locations for our matrices
		GLint modelLoc_Board = glGetUniformLocation(ourShaderBoard.Program, "model");
		GLint viewLoc_Board = glGetUniformLocation(ourShaderBoard.Program, "view");
		GLint projLoc_Board = glGetUniformLocation(ourShaderBoard.Program, "projection");

		// Pass locations to shaders ***
		glUniformMatrix4fv(viewLoc_Board, 1, GL_FALSE, glm::value_ptr(view_Board));
		glUniformMatrix4fv(projLoc_Board, 1, GL_FALSE, glm::value_ptr(projection_Board));

		// Draw container
		glBindVertexArray(VOA_Board);

		// function for printing the elements in a list

		bool isBlack = false;
		int temp = 0;
		bool count = 0;
		//list<glm::vec3>::iterator it;
		for (glm::vec3 it : cubePosList)
		{

			// Activate Image Textures
			if (((temp+(int)count) % 2) == 0)
			{
				// Activate specified texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureWhite);
				glUniform1i(glGetUniformLocation(ourShaderBoard.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate specified texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureBlack);
				glUniform1i(glGetUniformLocation(ourShaderBoard.Program, "ourTexture1"), 0);
			}
			temp++;
			if ((temp % 8) == 0) {
				count = !count;
			}
			
			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_Board(1.0f);
			model_Board = glm::translate(model_Board, (glm::vec3)it);
			//cubePosList.pop_front();
			GLfloat angle = -90.0f;
			model_Board = glm::rotate(model_Board, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc_Board, 1, GL_FALSE, glm::value_ptr(model_Board));

			glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
			//(GLenum mode, GLint first, GLsizei count)
		}

		for (glm::vec3 it : boarderPosList)
		{
			// ACTIVATE BORDER TEXTURE
			// Activate specified texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureBoarder);
			glUniform1i(glGetUniformLocation(ourShaderBoard.Program, "ourTexture1"), 0);

			glm::mat4 model_Board(1.0f);
			model_Board = glm::translate(model_Board, (glm::vec3)it);
			//cubePosList.pop_front();
			GLfloat angle = -90.0f;
			model_Board = glm::rotate(model_Board, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc_Board, 1, GL_FALSE, glm::value_ptr(model_Board));

			glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		}
		// *** CODE FOR CHESS BOARD *** //

		// Activate Shader
		ourShader.Use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
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

// Moves/alters the camera positions based on user input
// WASD and Arrow keys
void ProcessInput(GLFWwindow* window)
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
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
			keys[key] = false;
		}
	}
}

// GLFW: whenever the mouse moves, this callback is called
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// GLFW: whenever the mouse scroll wheel scrolls, this callback is called
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}
// *** CREATE 3D CHESS BOARD ON TERRAIN *** //