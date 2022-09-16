// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
//#include <GL/glew.h>

// GLFW
//#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2.h"
#include "SDL.h"
#include <stdio.h>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//// Function prototypes
//void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
//void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//SDL
SDL_Event event;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    /*glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

    //GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed

    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Delay(5);  // Pause execution for 3000 milliseconds, for example

    

    /*if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }*/

   /* glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);*/

    // Set the required callback functions
    /*glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);*/

    // Options, removes the mouse cursor for a more immersive experience
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

    // enable alpha support
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup and compile our shaders
    Shader ourShader("core.vs", "core.frag");

    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] =
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO

    // Load and create a texture
    GLuint texture;
    int width, height;
    // --== TEXTURE == --
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
   
    if (SOIL_load_image("image1.jpg", &width, &height, 0, SOIL_LOAD_RGBA) == 0) {
        std::cout << "Failed to create image in window" << std::endl;
    }
    unsigned char* image = SOIL_load_image("image1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    
    unsigned int lastTime = 0, currentTime;
    
    //int SDL_PollEvent(SDL_Event * event);
    while(true) {
        
        SDL_Event event;


        // Game loop

        while (SDL_PollEvent(&event))
        {

            // Set frame time
            currentTime = SDL_GetTicks();
            if (currentTime > lastTime + 10) {
                deltaTime = currentTime - lastTime;
                lastTime = currentTime;
            }

            // Check and call events
            //glfwPollEvents();
            DoMovement();

            // Clear the colorbuffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Draw our first triangle
            ourShader.Use();

            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

            glm::mat4 projection(1.0f);
            projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

            // Create camera transformation
            glm::mat4 view(1.0f);
            view = camera.GetViewMatrix();

            // Get the uniform locations
            GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
            GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
            GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

            // Pass the matrices to the shader
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(VAO);

            for (GLuint i = 0; i < 10; i++)
            {
                // Calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model;
                model = glm::translate(model, cubePositions[i]);
                GLfloat angle = 20.0f * i;
                model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glBindVertexArray(0);

            // Swap the buffers
            //glfwSwapBuffers(window);
        }
    }
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;

    //// Properly de-allocate all resources once they've outlived their purpose
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    ////glfwTerminate();

    //return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
            break;
        case SDLK_s:
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
            break;
        case SDLK_a:
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
            break;
        case SDLK_d:
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
            break;
        }
    }

    if (event.type == SDL_MOUSEMOTION) {
        GLfloat xOffset, yOffset;
        xOffset = event.motion.xrel;
        yOffset = event.motion.yrel;

        camera.ProcessMouseMovement(xOffset, -yOffset);
    }
    // Camera controls
    /*if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }*/
}

// Is called whenever a key is pressed/released via GLFW
//void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//
//    if (key >= 0 && key < 1024)
//    {
//        if (action == GLFW_PRESS)
//        {
//            keys[key] = true;
//        }
//        else if (action == GLFW_RELEASE)
//        {
//            keys[key] = false;
//        }
//    }
//}

//void MouseCallback(GLFWwindow* window, double xPos, double yPos)
//{
//    if (firstMouse)
//    {
//        lastX = xPos;
//        lastY = yPos;
//        firstMouse = false;
//    }
//
//    GLfloat xOffset = xPos - lastX;
//    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
//
//    lastX = xPos;
//    lastY = yPos;
//
//    camera.ProcessMouseMovement(xOffset, yOffset);
//}


//void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
//{
//    camera.ProcessMouseScroll(yOffset);
//}

 //it includes SDL_ShowCursor(SDL_DISABLE); and you dont need it anymore, + you need it for relative mouse mode

void pollEvents() {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_Event event;

    while (SDL_PollEvent(&event)) {


        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w:
                camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
                break;
            case SDLK_s:
                camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
                break;
            case SDLK_a:
                camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
                break;
            case SDLK_d:
                camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
                break;
            }
        }

        if (event.type == SDL_MOUSEMOTION) {
            GLfloat xOffset, yOffset;
            xOffset = event.motion.xrel;
            yOffset = event.motion.yrel;

            camera.ProcessMouseMovement(xOffset, -yOffset);
        }
    }
}