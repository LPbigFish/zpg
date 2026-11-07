/*
 * Copyright (c) 2026 Martin Němec
 *
 * File: main.cpp
 * Description: OpenGL programmable pipeline.
 */

//Include GLAD  
//Only define this in one file
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr



//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <string>
#include <iterator>
#include <iostream>
#include <vector>

#include "Models/plain.h"


// GLFW callback functions
static void error_callback(int error, const char* description) { fputs(description, stderr); }


GLuint createShaderFromFile(GLenum shaderType, const char* shaderFile)
{
	// Creates an empty shader
	GLuint shaderID = glCreateShader(shaderType);

	if (shaderID == 0)
	{
		std::cout << "Unable to create shader" << std::endl;
		exit(EXIT_FAILURE);
	}

	//Loading the contents of a file into a variable
	std::ifstream file(shaderFile);
	if (!file.is_open())
	{
		std::cout << "Unable to open file " << shaderFile << std::endl;
		glDeleteShader(shaderID);
		exit(-1);
	}
	std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	// Set the shader source code
	const char* source = shaderCode.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);
	
	// Compile the shader source code
	glCompileShader(shaderID);

	// Check specialization/compilation status
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
		std::cout
			<< "Shader failed:\n"
			<< infoLog << std::endl;
		glDeleteShader(shaderID);
		exit(1);
	}
	return shaderID;
}


int main(void)
{
	// Pointer to the GLFW window
	GLFWwindow* window;
	
	// Initialize GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	// Initialize GLAD and load OpenGL function pointers
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		printf("GLAD initialization failed\n");
		return -1;
	}

	// Get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);
		
	// Set GLFW callback functions
	glfwSetErrorCallback(error_callback);

	// Get framebuffer size and set the viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float)height;
	glViewport(0, 0, width, height);


	//vertex buffer object (VBO)
	GLuint VBO = 0;
	glGenBuffers(1, &VBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plain), plain, GL_STATIC_DRAW);

	//Vertex Array Object (VAO)
	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO); //generate the VAO
	glBindVertexArray(VAO); //bind the VAO
	glEnableVertexAttribArray(0); //enable vertex attributes
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// index, number of components, data type, normalized, vertex stride, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));



	// Activate the texture unit and load the texture.
	int text_width, text_height, channels;
	GLuint tex;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("texture/wooden_fence.png", &text_width, &text_height, &channels, 4);
	if (!data) {
		std::cerr << "Error loading texture: wooden_fence.png" << std::endl;
	}
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);


	// Create and compile the vertex and fragment shaders
	GLuint vertexShader = createShaderFromFile(GL_VERTEX_SHADER, "shaders/basic.vert");
	GLuint fragmentShader =	createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/basic.frag");
	
	//Create and link the shader program 
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, vertexShader);
	glLinkProgram(shaderProgram);

	glm::mat4 M = glm::mat4(1.0f);
	GLint locationModelMatrix;

	locationModelMatrix = glGetUniformLocation(shaderProgram, "modelMatrix");
	
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		M = glm::scale(glm::mat4(1.0f), glm::vec3(0.5));
		M = glm::rotate(M,(float)glfwGetTime(), glm::vec3(1, 1, 0));
		glUniformMatrix4fv(locationModelMatrix, 1, GL_FALSE, glm::value_ptr(M));
		// Draw a triangles
		glDrawArrays(GL_TRIANGLES, 0, 6); //mode,first,count
		// Display the rendered frame and process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Clean up and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}