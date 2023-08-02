#pragma once

#include <glad/glad.h>
#include <iostream>

class Shader
{
	
public:
	Shader() = default;
	Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	Shader(const char* computeShaderFilePath);
	uint32_t programID = 0;	
};