#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "Camera.h"

const std::string shaderFilePath = "..\\RayTracer\\Shaders\\";
static const uint32_t maxNumShaders = 3;

class Engine
{

public:
	Engine();
	
	void Init();
	void RenderParallel(GLFWwindow* window);
	void RenderNormal(GLFWwindow* window);
	void Move(float distance, int shaderID);
	void Turn(float angle, int shaderID);
	void SetTime(float t, int shaderID);

private:
	
	// vertex, fragment, compute shaders
	Shader* shader[maxNumShaders];
	GLuint quadvbo = 0; // vertex buffer object
	GLuint vao = 0; // vertex array object
	const uint32_t textureWidth = 512, textureHeight = 512; // texture dimensions for rendering ray trace
	GLuint outputTexture; // texture to write to
	Camera* cam;
	// quad for full screen ray traced rendering
	float quadpoints[20] = {
   -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
};