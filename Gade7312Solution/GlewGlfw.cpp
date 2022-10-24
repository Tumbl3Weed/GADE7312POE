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
Camera camera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -128.1f, -42.4f);
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024]; // Array of 1024 different types of keys
bool firstMouse = true; // Only handling one type of mouse, thus true

// Timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Animate 
bool animating = false;
bool cameraLocked;

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
	unsigned char* imageWhite = SOIL_load_image("res/images/WhiteMarbleTexture.jpg", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

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
	unsigned char* imageBlack = SOIL_load_image("res/images/Blackmarbletexture.jpg", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

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

			cubePosList.push_back(glm::vec3((float)x - 4, randomHeight, (float)z - 4));
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
				boarderPosList.push_back(glm::vec3(tempX - 4, 0.0f, tempZ - 4));
			}
			if (x == 0 && z == 0) {
				boarderPosList.push_back(glm::vec3(tempX - 4, 0.0f, tempZ + 1 - 4));
				boarderPosList.push_back(glm::vec3(tempX - 4 + 1, 0.0f, tempZ - 4));
			}
			if (x == 7 && z == 0) {
				boarderPosList.push_back(glm::vec3(tempX - 4, 0.0f, tempZ + 1 - 4));
				boarderPosList.push_back(glm::vec3(tempX - 4 - 1, 0.0f, tempZ - 4));
			}
			if (x == 0 && z == 7) {
				boarderPosList.push_back(glm::vec3(tempX - 4, 0.0f, tempZ - 1 - 4));
				boarderPosList.push_back(glm::vec3(tempX - 4 + 1, 0.0f, tempZ - 4));
			}
			if (x == 7 && z == 7) {
				boarderPosList.push_back(glm::vec3(tempX - 4, 0.0f, tempZ - 1 - 4));
				boarderPosList.push_back(glm::vec3(tempX - 4 - 1, 0.0f, tempZ - 4));
			}
		}
	}

	//Build & Compile Shader Program
	Shader ourShader("core.vs", "core.frag");

	//*********TERRAIN TEXTURE********************
	//Create and load texture replace it with your own image path.
	int width, height, nrChannels;

	unsigned char* data = SOIL_load_image("res/images/bitmapheightmapSmall.png", &width, &height, &nrChannels, 0);

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
	GLfloat yScale = 0.05f; //normalize the height map data and scale it to the desired height
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
			vertices.push_back(-width / 2.0f + width * i / (float)width); // vx
			//vertices.push_back(-height / 2.0f + height * i / (float)height); // vx

			vertices.push_back((int)y * yScale - 10); // vy
			vertices.push_back(-height / 2.0f + height * j / (float)height); // vz
			//vertices.push_back(-width / 2.0f + width * j / (float)width); // vz
		}
	}
	//cout << "Loaded " << vertices.size() / 3 << " vertices" << endl;
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
	//cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << endl;
	//cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << endl;

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

	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	GLuint textureHM1, textureHM2, textureHM3, textureHM4;

#pragma region HM Texture 1
	// Create and load HM texture
	glGenTextures(1, &textureHM1);
	glBindTexture(GL_TEXTURE_2D, textureHM1);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* HM_Image = SOIL_load_image("res/images/terrain1.png", &width, &height, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, HM_Image);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(HM_Image);
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

#pragma region HM Texture 2
	// Create and load HM texture
	glGenTextures(1, &textureHM2);
	glBindTexture(GL_TEXTURE_2D, textureHM2);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* HM_Image2 = SOIL_load_image("res/images/terrain1.png", &width, &height, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, HM_Image2);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(HM_Image2);
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

#pragma region HM Texture 3
	// Create and load HM texture
	glGenTextures(1, &textureHM3);
	glBindTexture(GL_TEXTURE_2D, textureHM3);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* HM_Image3 = SOIL_load_image("res/images/terrain1.png", &width, &height, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, HM_Image3);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(HM_Image3);
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

#pragma region HM Texture 4
	// Create and load HM texture
	glGenTextures(1, &textureHM4);
	glBindTexture(GL_TEXTURE_2D, textureHM4);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* HM_Image4 = SOIL_load_image("res/images/terrain1.png", &width, &height, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, HM_Image4);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(HM_Image4);
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

	// *** CODE FOR CHESS PIECE - Knight *** //
#pragma region CODE FOR CHESS PIECE - Knight

//Build & Compile Shader Program for Chessboard
	Shader ourShaderKnight("coreBoard.vs", "coreBoard.frag");

	// Vertex data for our Knight piece
	GLfloat verticesKnight[14436];

	// *** Read Vertex data from Knight.txt file ***//
	ifstream myFileKnight("Knight.txt");
	int i = 0;

	if (myFileKnight.is_open())
	{
		string line;

		while (!myFileKnight.eof())
		{
			getline(myFileKnight, line, ' ');
			verticesKnight[i] = stof(line);
			i++;
			getline(myFileKnight, line, ' ');
			verticesKnight[i] = stof(line);
			i++;
			getline(myFileKnight, line, '\n');
			verticesKnight[i] = stof(line);
			i++;
		}
		myFileKnight.close();
	}
	else
	{
		cout << "Can't open the file";
	}
	// *** Read Vertex data from Knight.txt file ***//

	// Positions of Knight
	glm::vec3 KnightPositions[] =
	{
		// Row 1
		glm::vec3(-3.0f, 0.5f, 3.0f),
		glm::vec3(2.0f, 0.5f, 3.0f),

		// Row 2
		glm::vec3(-3.0f, 0.5f, -4.0f),
		glm::vec3(2.0f, 0.5f, -4.0f),

	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_Knight, VOA_Knight;
	glGenVertexArrays(1, &VOA_Knight);
	glGenBuffers(1, &VBA_Knight);

	// Bind the vertex array object
	glBindVertexArray(VOA_Knight);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_Knight);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesKnight), verticesKnight, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); //Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

#pragma endregion
	// *** CODE FOR CHESS PIECE - ROOK *** //
#pragma region CODE FOR CHESS PIECE - ROOK

	//Build & Compile Shader Program for Chessboard
	Shader ourShaderRook("coreBoard.vs", "coreBoard.frag");

	// Vertex data for our pawn piece
	GLfloat verticesRook[13716];

	// *** Read Vertex data from Rook.txt file ***//
	ifstream myFileRook("Rook.txt");
	i = 0;

	if (myFileRook.is_open())
	{
		string line;

		while (!myFileRook.eof())
		{
			getline(myFileRook, line, ' ');
			verticesRook[i] = stof(line);
			i++;
			getline(myFileRook, line, ' ');
			verticesRook[i] = stof(line);
			i++;
			getline(myFileRook, line, '\n');
			verticesRook[i] = stof(line);
			i++;
		}
		myFileRook.close();
	}
	else
	{
		cout << "Can't open the file";
	}
	// *** Read Vertex data from Rook.txt file ***//

	// Positions of rooks
	glm::vec3 RookPositions[] =
	{
		// Row 1
		glm::vec3(-4.0f, 0.5f, 3.0f),
		glm::vec3(3.0f, 0.5f, 3.0f),

		// Row 2
		glm::vec3(-4.0f, 0.5f, -4.0f),
		glm::vec3(3.0f, 0.5f, -4.0f),
	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_Rook, VOA_Rook;
	glGenVertexArrays(1, &VOA_Rook);
	glGenBuffers(1, &VBA_Rook);

	// Bind the vertex array object
	glBindVertexArray(VOA_Rook);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_Rook);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRook), verticesRook, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); //Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

	//// Chess Piece Pawn texture variables
	//GLuint rookTextureW, rookTextureB;
	//int widthRook, heightRook;
#pragma endregion
	// *** CODE FOR CHESS PIECE - BISHOP *** //
#pragma region CODE FOR CHESS PIECE - Bishop

//Build & Compile Shader Program for Chessboard
	Shader ourShaderBishop("coreBoard.vs", "coreBoard.frag");

	// Vertex data for our Bishop piece
	GLfloat verticesBishop[14436];

	// *** Read Vertex data from Bishop.txt file ***//
	ifstream myFileBishop("Bishop.txt");
	i = 0;

	if (myFileBishop.is_open())
	{
		string line;

		while (!myFileBishop.eof())
		{
			getline(myFileBishop, line, ' ');
			verticesBishop[i] = stof(line);
			i++;
			getline(myFileBishop, line, ' ');
			verticesBishop[i] = stof(line);
			i++;
			getline(myFileBishop, line, '\n');
			verticesBishop[i] = stof(line);
			i++;
		}
		myFileBishop.close();
	}
	else
	{
		cout << "Can't open the file";
	}
	// *** Read Vertex data from Bishop.txt file ***//

	// Positions of Bishop
	glm::vec3 BishopPositions[] =
	{
		// Row 1
		glm::vec3(-2.0f, 0.5f, 3.0f),
		glm::vec3(1.0f, 0.5f, 3.0f),


		// Row 2
		glm::vec3(-2.0f, 0.5f, -4.0f),
		glm::vec3(1.0f, 0.5f, -4.0f),

	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_Bishop, VOA_Bishop;
	glGenVertexArrays(1, &VOA_Bishop);
	glGenBuffers(1, &VBA_Bishop);

	// Bind the vertex array object
	glBindVertexArray(VOA_Bishop);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_Bishop);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBishop), verticesBishop, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); //Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

#pragma endregion
	// *** CODE FOR CHESS PIECE - PAWN *** //
#pragma region CODE FOR CHESS PIECE - PAWN

	//Build & Compile Shader Program for Chessboard
	Shader ourShaderPawn("coreBoard.vs", "coreBoard.frag");

	// Vertex data for our pawn piece
	GLfloat verticesPawn[4716];

	// *** Read Vertex data from Pawn.txt file ***//
	ifstream myFilePawn("Pawn.txt");
	i = 0;

	if (myFilePawn.is_open())
	{
		string line;

		while (!myFilePawn.eof())
		{
			getline(myFilePawn, line, ' ');
			verticesPawn[i] = stof(line);
			i++;
			getline(myFilePawn, line, ' ');
			verticesPawn[i] = stof(line);
			i++;
			getline(myFilePawn, line, '\n');
			verticesPawn[i] = stof(line);
			i++;
		}
		myFilePawn.close();
	}
	else
	{
		cout << "Can't open the file";
	}
	// *** Read Vertex data from Pawn.txt file ***//

	// Positions of pawns
	glm::vec3 pawnPositions[] =
	{
		// Row 1
		glm::vec3(-4.0f, 0.5f, 2.0f),
		glm::vec3(-3.0f, 0.5f, 2.0f),
		glm::vec3(-2.0f, 0.5f, 2.0f),
		glm::vec3(-1.0f, 0.5f, 2.0f),
		glm::vec3(0.0f, 0.5f, 2.0f),
		glm::vec3(1.0f, 0.5f, 2.0f),
		glm::vec3(2.0f, 0.5f, 2.0f),
		glm::vec3(3.0f, 0.5f, 2.0f),

		// Row 2
		glm::vec3(-4.0f, 0.5f, -3.0f),
		glm::vec3(-3.0f, 0.5f, -3.0f),
		glm::vec3(-2.0f, 0.5f, -3.0f),
		glm::vec3(-1.0f, 0.5f, -3.0f),
		glm::vec3(0.0f, 0.5f, -3.0f),
		glm::vec3(1.0f, 0.5f, -3.0f),
		glm::vec3(2.0f, 0.5f, -3.0f),
		glm::vec3(3.0f, 0.5f, -3.0f),
	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_Pawn, VOA_Pawn;
	glGenVertexArrays(1, &VOA_Pawn);
	glGenBuffers(1, &VBA_Pawn);

	// Bind the vertex array object
	glBindVertexArray(VOA_Pawn);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_Pawn);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPawn), verticesPawn, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); //Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

	// Chess Piece Pawn texture variables
	GLuint pawnTextureW, pawnTextureB;
	int widthPawn, heightPawn;

#pragma region Pawn Texture White
	// Create and load White texture
	glGenTextures(1, &pawnTextureW);
	glBindTexture(GL_TEXTURE_2D, pawnTextureW);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* pawnImageW = SOIL_load_image("res/images/white.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, pawnImageW);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(pawnImageW);
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

#pragma region Pawn Texture Black
	// Create and load White texture
	glGenTextures(1, &pawnTextureB);
	glBindTexture(GL_TEXTURE_2D, pawnTextureB);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Actual texture loading code
	unsigned char* pawnImageB = SOIL_load_image("res/images/black.png", &widthB, &heightB, 0, SOIL_LOAD_RGBA);

	// Specify 2D texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthB, heightB, 0, GL_RGBA, GL_UNSIGNED_BYTE, pawnImageB);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(pawnImageB);
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

#pragma endregion
	// ^*** CODE FOR CHESS PIECE - PAWN ***^ //

	// *** CODE FOR CHESS PIECE - King *** //
#pragma region CODE FOR CHESS PIECE - King

//Build & Compile Shader Program for Chessboard
	Shader ourShaderKing("coreBoard.vs", "coreBoard.frag");

	// Vertex data for our King piece
	GLfloat verticesKing[14436];

	// *** Read Vertex data from King.txt file ***//
	ifstream myFileKing("King.txt");
	i = 0;

	if (myFileKing.is_open())
	{
		string line;

		while (!myFileKing.eof())
		{
			getline(myFileKing, line, ' ');
			verticesKing[i] = stof(line);
			i++;
			getline(myFileKing, line, ' ');
			verticesKing[i] = stof(line);
			i++;
			getline(myFileKing, line, '\n');
			verticesKing[i] = stof(line);
			i++;
		}
		myFileKing.close();
	}
	else
	{
		cout << "Can't open the file";
	}
	// *** Read Vertex data from King.txt file ***//

	// Positions of King
	glm::vec3 KingPositions[] =
	{
		// Row 1
		glm::vec3(0.0f, 0.5f, 3.0f),
		//glm::vec3(1.0f, 0.5f, 3.0f),


		// Row 2
		glm::vec3(0.0f, 0.5f, -4.0f),
		//glm::vec3(1.0f, 0.5f, -4.0f),

	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_King, VOA_King;
	glGenVertexArrays(1, &VOA_King);
	glGenBuffers(1, &VBA_King);

	// Bind the vertex array object
	glBindVertexArray(VOA_King);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_King);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesKing), verticesKing, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); //Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

#pragma endregion

	// *** CODE FOR CHESS PIECE - QUeen *** //
#pragma region CODE FOR CHESS PIECE - Queen

//Build & Compile Shader Program for Chessboard
	Shader ourShaderQueen("coreBoard.vs", "coreBoard.frag");

	// Vertex data for our Queen piece
	GLfloat verticesQueen[14436];

	// *** Read Vertex data from Queen.txt file ***//
	ifstream myFileQueen("Queen.txt");
	i = 0;

	if (myFileQueen.is_open())
	{
		string line;

		while (!myFileQueen.eof())
		{
			getline(myFileQueen, line, ' ');
			verticesQueen[i] = stof(line);
			i++;
			getline(myFileQueen, line, ' ');
			verticesQueen[i] = stof(line);
			i++;
			getline(myFileQueen, line, '\n');
			verticesQueen[i] = stof(line);
			i++;
		}
		myFileQueen.close();
	}
	else
	{
		cout << "Can't open the file";
	}
	// *** Read Vertex data from Queen.txt file ***//

	// Positions of Queen
	glm::vec3 QueenPositions[] =
	{
		// Row 1
		glm::vec3(-1.0f, 0.5f, 3.0f),
		//glm::vec3(1.0f, 0.5f, 3.0f),


		// Row 2
		glm::vec3(-1.0f, 0.5f, -4.0f),
		//glm::vec3(1.0f, 0.5f, -4.0f),

	};

	// Generate the vertex arrays and vertex buffers and save them into variables
	GLuint VBA_Queen, VOA_Queen;
	glGenVertexArrays(1, &VOA_Queen);
	glGenBuffers(1, &VBA_Queen);

	// Bind the vertex array object
	glBindVertexArray(VOA_Queen);

	// Bind and set the vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBA_Queen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQueen), verticesQueen, GL_STATIC_DRAW);

	// Create the vertex pointer and enable the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //Position
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); //Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); //Texture
	glEnableVertexAttribArray(2);

	// Unbind the vertex array to prevent strange bugs
	glBindVertexArray(0);

#pragma endregion

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

		// *** CODE FOR CHESS CHESS PIECE - PAWN WHILE LOOP *** //
#pragma region CODE FOR CHESS PIECE - PAWN WHILE LOOP

		// Activate Shader
		ourShaderPawn.Use();

		// Create Projection Matrix
		glm::mat4 projection_Pawn(1.0f);
		//Perspective view ***
		projection_Pawn = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// Create camera transformation
		glm::mat4 view_Pawn(1.0f);
		view_Pawn = camera.GetViewMatrix(cameraLocked);

		// Get the uniform locations for our matrices
		GLint modelLoc_Pawn = glGetUniformLocation(ourShaderPawn.Program, "model");
		GLint viewLoc_Pawn = glGetUniformLocation(ourShaderPawn.Program, "view");
		GLint projLoc_Pawn = glGetUniformLocation(ourShaderPawn.Program, "projection");

		// Pass locations to shaders
		glUniformMatrix4fv(viewLoc_Pawn, 1, GL_FALSE, glm::value_ptr(view_Pawn));
		glUniformMatrix4fv(projLoc_Pawn, 1, GL_FALSE, glm::value_ptr(projection_Pawn));

		// Draw container
		glBindVertexArray(VOA_Pawn);

		for (GLuint i = 0; i < 16; i++)
		{
			if (i < 8)
			{
				// Activate White Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureW);
				glUniform1i(glGetUniformLocation(ourShaderPawn.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate Black Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureB);
				glUniform1i(glGetUniformLocation(ourShaderPawn.Program, "ourTexture1"), 0);
			}

			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_Pawn(1.0f);
			model_Pawn = glm::translate(model_Pawn, pawnPositions[i]);
			if (animating) // Select which pawns are animated
			{
				model_Pawn = glm::rotate(model_Pawn, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
			{
				model_Pawn = glm::rotate(model_Pawn, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			glUniformMatrix4fv(modelLoc_Pawn, 1, GL_FALSE, glm::value_ptr(model_Pawn));

			glDrawArrays(GL_TRIANGLES, 0, 4716);
		}
#pragma endregion
		// *** CODE FOR CHESS CHESS PIECE - KNIGHT WHILE LOOP *** //
#pragma region CODE FOR CHESS PIECE - KNIGHT WHILE LOOP
		// Activate Shader
		ourShaderKnight.Use();

		// Create Projection Matrix
		glm::mat4 projection_Knight(1.0f);
		//Perspective view ***
		projection_Knight = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// Create camera transformation
		glm::mat4 view_Knight(1.0f);
		view_Knight = camera.GetViewMatrix(cameraLocked);

		// Get the uniform locations for our matrices
		GLint modelLoc_Knight = glGetUniformLocation(ourShaderKnight.Program, "model");
		GLint viewLoc_Knight = glGetUniformLocation(ourShaderKnight.Program, "view");
		GLint projLoc_Knight = glGetUniformLocation(ourShaderKnight.Program, "projection");

		// Pass locations to shaders
		glUniformMatrix4fv(viewLoc_Knight, 1, GL_FALSE, glm::value_ptr(view_Knight));
		glUniformMatrix4fv(projLoc_Knight, 1, GL_FALSE, glm::value_ptr(projection_Knight));

		// Draw container
		glBindVertexArray(VOA_Knight);

		for (GLuint i = 0; i < 4; i++)
		{
			if (i < 2)
			{
				// Activate White Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureW);
				glUniform1i(glGetUniformLocation(ourShaderKnight.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate Black Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureB);
				glUniform1i(glGetUniformLocation(ourShaderKnight.Program, "ourTexture1"), 0);
			}

			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_Knight(1.0f);

			model_Knight = glm::translate(model_Knight, KnightPositions[i]);
			if (animating) // Select which pawns are animated
			{
				model_Knight = glm::rotate(model_Knight, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
			{
				model_Knight = glm::rotate(model_Knight, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			glUniformMatrix4fv(modelLoc_Knight, 1, GL_FALSE, glm::value_ptr(model_Knight));

			glDrawArrays(GL_TRIANGLES, 0, 14436);
		}
#pragma endregion
		// *** CODE FOR CHESS CHESS PIECE - ROOK WHILE LOOP *** //
#pragma region CODE FOR CHESS PIECE - ROOK WHILE LOOP

		// Activate Shader
		ourShaderRook.Use();

		// Create Projection Matrix
		glm::mat4 projection_Rook(1.0f);
		//Perspective view ***
		projection_Rook = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// Create camera transformation
		glm::mat4 view_Rook(1.0f);
		view_Rook = camera.GetViewMatrix(cameraLocked);

		// Get the uniform locations for our matrices
		GLint modelLoc_Rook = glGetUniformLocation(ourShaderPawn.Program, "model");
		GLint viewLoc_Rook = glGetUniformLocation(ourShaderPawn.Program, "view");
		GLint projLoc_Rook = glGetUniformLocation(ourShaderPawn.Program, "projection");

		// Pass locations to shaders
		glUniformMatrix4fv(viewLoc_Rook, 1, GL_FALSE, glm::value_ptr(view_Rook));
		glUniformMatrix4fv(projLoc_Rook, 1, GL_FALSE, glm::value_ptr(projection_Rook));

		// Draw container
		glBindVertexArray(VOA_Rook);

		for (GLuint i = 0; i < 4; i++)
		{
			if (i < 2)
			{
				// Activate White Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureW);
				glUniform1i(glGetUniformLocation(ourShaderRook.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate Black Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureB);
				glUniform1i(glGetUniformLocation(ourShaderRook.Program, "ourTexture1"), 0);
			}

			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_Rook(1.0f);

			model_Rook = glm::translate(model_Rook, RookPositions[i]);
			if (animating) // Select which pawns are animated
			{
				model_Rook = glm::rotate(model_Rook, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
			{
				model_Rook = glm::rotate(model_Rook, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			glUniformMatrix4fv(modelLoc_Rook, 1, GL_FALSE, glm::value_ptr(model_Rook));

			glDrawArrays(GL_TRIANGLES, 0, 13716);
		}
#pragma endregion


#pragma region CODE FOR CHESS PIECE - Bishop WHILE LOOP
		// Activate Shader
		ourShaderBishop.Use();

		// Create Projection Matrix
		glm::mat4 projection_Bishop(1.0f);
		//Perspective view ***
		projection_Bishop = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// Create camera transformation
		glm::mat4 view_Bishop(1.0f);
		view_Bishop = camera.GetViewMatrix(cameraLocked);

		// Get the uniform locations for our matrices
		GLint modelLoc_Bishop = glGetUniformLocation(ourShaderBishop.Program, "model");
		GLint viewLoc_Bishop = glGetUniformLocation(ourShaderBishop.Program, "view");
		GLint projLoc_Bishop = glGetUniformLocation(ourShaderBishop.Program, "projection");

		// Pass locations to shaders
		glUniformMatrix4fv(viewLoc_Bishop, 1, GL_FALSE, glm::value_ptr(view_Bishop));
		glUniformMatrix4fv(projLoc_Bishop, 1, GL_FALSE, glm::value_ptr(projection_Bishop));

		// Draw container
		glBindVertexArray(VOA_Bishop);

		for (GLuint i = 0; i < 4; i++)
		{
			if (i < 2)
			{
				// Activate White Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureW);
				glUniform1i(glGetUniformLocation(ourShaderBishop.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate Black Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureB);
				glUniform1i(glGetUniformLocation(ourShaderBishop.Program, "ourTexture1"), 0);
			}

			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_Bishop(1.0f);

			model_Bishop = glm::translate(model_Bishop, BishopPositions[i]);
			if (animating) // Select which pawns are animated
			{
				model_Bishop = glm::rotate(model_Bishop, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
			{
				model_Bishop = glm::rotate(model_Bishop, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			glUniformMatrix4fv(modelLoc_Bishop, 1, GL_FALSE, glm::value_ptr(model_Bishop));

			glDrawArrays(GL_TRIANGLES, 0, 14436);
		}
#pragma endregion

#pragma region CODE FOR CHESS PIECE - King WHILE LOOP
		// Activate Shader
		ourShaderKing.Use();

		// Create Projection Matrix
		glm::mat4 projection_King(1.0f);
		//Perspective view ***
		projection_King = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// Create camera transformation
		glm::mat4 view_King(1.0f);
		view_King = camera.GetViewMatrix(cameraLocked);

		// Get the uniform locations for our matrices
		GLint modelLoc_King = glGetUniformLocation(ourShaderKing.Program, "model");
		GLint viewLoc_King = glGetUniformLocation(ourShaderKing.Program, "view");
		GLint projLoc_King = glGetUniformLocation(ourShaderKing.Program, "projection");

		// Pass locations to shaders
		glUniformMatrix4fv(viewLoc_King, 1, GL_FALSE, glm::value_ptr(view_King));
		glUniformMatrix4fv(projLoc_King, 1, GL_FALSE, glm::value_ptr(projection_King));

		// Draw container
		glBindVertexArray(VOA_King);

		for (GLuint i = 0; i < 2; i++)
		{
			if (i < 1)
			{
				// Activate White Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureW);
				glUniform1i(glGetUniformLocation(ourShaderKing.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate Black Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureB);
				glUniform1i(glGetUniformLocation(ourShaderKing.Program, "ourTexture1"), 0);
			}

			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_King(1.0f);
			model_King = glm::translate(model_King, KingPositions[i]);
			if (animating) // Select which pawns are animated
			{
				model_King = glm::rotate(model_King, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
			{
				model_King = glm::rotate(model_King, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}
			glUniformMatrix4fv(modelLoc_King, 1, GL_FALSE, glm::value_ptr(model_King));
			glDrawArrays(GL_TRIANGLES, 0, 14436);
		}
#pragma endregion

#pragma region CODE FOR CHESS PIECE - Queen WHILE LOOP
		// Activate Shader
		ourShaderQueen.Use();

		// Create Projection Matrix
		glm::mat4 projection_Queen(1.0f);
		//Perspective view ***
		projection_Queen = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// Create camera transformation
		glm::mat4 view_Queen(1.0f);
		view_Queen = camera.GetViewMatrix(cameraLocked);

		// Get the uniform locations for our matrices
		GLint modelLoc_Queen = glGetUniformLocation(ourShaderQueen.Program, "model");
		GLint viewLoc_Queen = glGetUniformLocation(ourShaderQueen.Program, "view");
		GLint projLoc_Queen = glGetUniformLocation(ourShaderQueen.Program, "projection");

		// Pass locations to shaders
		glUniformMatrix4fv(viewLoc_Queen, 1, GL_FALSE, glm::value_ptr(view_Queen));
		glUniformMatrix4fv(projLoc_Queen, 1, GL_FALSE, glm::value_ptr(projection_Queen));

		// Draw container
		glBindVertexArray(VOA_Queen);

		for (GLuint i = 0; i < 2; i++)
		{
			if (i < 1)
			{
				// Activate White Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureW);
				glUniform1i(glGetUniformLocation(ourShaderQueen.Program, "ourTexture1"), 0);
			}
			else
			{
				// Activate Black Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pawnTextureB);
				glUniform1i(glGetUniformLocation(ourShaderQueen.Program, "ourTexture1"), 0);
			}

			// Calculate the model matrix for each object and pass it to the shader before drawing
			glm::mat4 model_Queen(1.0f);

			model_Queen = glm::translate(model_Queen, QueenPositions[i]);
			if (animating) // Select which pawns are animated
			{
				model_Queen = glm::rotate(model_Queen, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
			{
				model_Queen = glm::rotate(model_Queen, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			glUniformMatrix4fv(modelLoc_Queen, 1, GL_FALSE, glm::value_ptr(model_Queen));

			glDrawArrays(GL_TRIANGLES, 0, 14436);
		}
#pragma endregion

		// *** CODE FOR CHESS BOARD *** //
		ourShaderBoard.Use();

		// Create Projection Matrix *** (moved into while loop in order to update zoom)
		glm::mat4 projection_Board(1.0f);
		//Perspective view ***
		projection_Board = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

		// Create camera transformation ***
		glm::mat4 view_Board(1.0f);
		view_Board = camera.GetViewMatrix(cameraLocked);

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
			if (((temp + (int)count) % 2) == 0)
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
		glm::mat4 view = camera.GetViewMatrix(cameraLocked);
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
			if (indices[strip] % 3 == 0.0f)
			{
				//Activate HM texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureHM1);
				glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
			}
			else if (indices[strip] % 4 == 0.0f)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureHM2);
				glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
			}
			else if (indices[strip] % 5 == 0.0f)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureHM3);
				glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureHM4);
				glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
			}

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

// Animate the rotation of the Chess Piece
// Animate the position of the Chess Pieces
glm::vec3 AnimatePawnPos(glm::vec3 pos)
{
	if (animating)
	{
		return glm::vec3(pos.x, pos.y + 1, pos.z);
	}
	return pos;
}
//*** Chess Piece Creation & Animation Code Snippets ***//

// Moves/alters the camera positions based on user input
// WASD and Arrow keys

void ProcessInput(GLFWwindow* window)
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		if (!cameraLocked)
			camera.ProcessKeyboard(FORWARD, deltaTime / 2);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		if (!cameraLocked)
			camera.ProcessKeyboard(BACKWARD, deltaTime / 2);
	}

	if (keys[GLFW_KEY_A])
	{
		if (!cameraLocked)
			camera.ProcessKeyboard(LEFT, deltaTime / 2);
	}


	if (keys[GLFW_KEY_D])
	{
		if (!cameraLocked)
			camera.ProcessKeyboard(RIGHT, deltaTime / 2);
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
			if (keys[GLFW_KEY_TAB]) {
				cameraLocked = !cameraLocked;
			}
			if (keys[GLFW_KEY_LEFT])
			{
				cameraLocked = true;
				camera.ProcessKeyboard(LEFT);
			}
			if (keys[GLFW_KEY_RIGHT])
			{
				cameraLocked = true;
				camera.ProcessKeyboard(RIGHT);
			}
			keys[key] = false;
		}
	}

	// Spacebar to start anim
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animating = !animating;
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