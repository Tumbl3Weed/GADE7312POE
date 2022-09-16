#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
using namespace std;

class Shader
{
public:
	GLuint Program;

	Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
	{
		//Retrieve vertex and fragment source code from file paths
		string vertexStringStream;
		string fragmentShaderStream;
		ifstream vShaderBuffer;
		ifstream fShaderBuffer;

		//Exception handling
		vShaderBuffer.exceptions(ifstream::badbit);
		fShaderBuffer.exceptions(ifstream::badbit);

		try
		{
			//Open files
			vShaderBuffer.open(vertexPath);
			fShaderBuffer.open(fragmentPath);

			stringstream vShaderStream, fShaderStream;

			//Store file contents into streams
			vShaderStream << vShaderBuffer.rdbuf();
			fShaderStream << fShaderBuffer.rdbuf();

			//Close files
			vShaderBuffer.close();
			fShaderBuffer.close();

			//Convert streams into strings
			vertexStringStream = vShaderStream.str();
			fragmentShaderStream = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}

		const GLchar *vShaderCode = vertexStringStream.c_str();
		const GLchar *fShaderCode = fragmentShaderStream.c_str();

		//BUILD AND COMPILE OUR SHADER PROGRAM

		//Compile time errors variables
		GLuint iVShader, iFShader;
		GLint success;
		GLchar infoLog[512];

		//Build our vertex shader
		iVShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(iVShader, 1, &vShaderCode, NULL);

		//Compile our shader
		glCompileShader(iVShader);

		//Check for compile time errorsand save the status in success variable
		glGetShaderiv(iVShader, GL_COMPILE_STATUS, &success);

		//Check if the shader has successfully compiled
		if (!success)
		{
			glGetShaderInfoLog(iVShader, 512, NULL, infoLog); // Get compilation errors and save in infoLog variable
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}

		//Build our fragment shader
		iFShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(iFShader, 1, &fShaderCode, NULL);

		//Compile our fragment shader
		glCompileShader(iFShader);

		//Check for compile time errorsand save the status in success variable
		glGetShaderiv(iFShader, GL_COMPILE_STATUS, &success);

		//Check if the shader has successfully compiled
		if (!success)
		{
			glGetShaderInfoLog(iFShader, 512, NULL, infoLog); // Get compilation errors and save in infoLog variable
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}

		//Link the Shaders
		this->Program = glCreateProgram();
		glAttachShader(this->Program, iVShader);
		glAttachShader(this->Program, iFShader);
		glLinkProgram(this->Program);

		//Check for linking errorsand save in success variable
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);

		//Check if the shader has successfully linked
		if (!success)
		{
			glGetShaderInfoLog(this->Program, 512, NULL, infoLog); // Get compilation errors and save in infoLog variable
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}

		//Shaders have linked successfullyand we can now delete the individual shaders as they are linked in shaderProgram
		glDeleteShader(iVShader);
		glDeleteShader(iFShader);
	}

	void Use()
	{
		glUseProgram(this->Program);
	}
};
#endif