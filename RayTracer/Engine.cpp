#include "Engine.h"
#include <glm/glm.hpp>

Engine::Engine()
{
	shader[0] = new Shader((shaderFilePath + "RayTracer.vs").c_str(), (shaderFilePath + "Raytracer.fs").c_str());
	shader[1] = new Shader((shaderFilePath + "RayTracerComputeSH.cs").c_str());
    shader[2] = new Shader((shaderFilePath + "RayTracer.vs").c_str(), (shaderFilePath + "RayTracerV2.fs").c_str());

    glUseProgram(shader[1]->programID);
    glUniform1i(glGetUniformLocation(shader[1]->programID, "screenImageData"), 0);
    Init();

    cam = new Camera();
}

void Engine::Init()
{   
    // create the vertex buffer object  
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &quadvbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadpoints), &quadpoints, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(vao);
   
    
    // init the texture for screen rendering
    glGenTextures(1, &outputTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, outputTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

  
}

void Engine::RenderParallel(GLFWwindow* window)
{
      

    // use our compute shader
    glUseProgram(shader[1]->programID);
    glDispatchCompute((GLuint)textureWidth, (GLuint)textureHeight, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader[0]->programID);
   
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
}

void Engine::RenderNormal(GLFWwindow* window)
{
    glUseProgram(shader[2]->programID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
}


void Engine::Move(float distance, int shaderID)
{
    if (shaderID > maxNumShaders-1)
        return;

    glUseProgram(shader[shaderID]->programID);
    // get our camera position from compute shader
    int location = glGetUniformLocation(shader[shaderID]->programID, "camPos");
    GLfloat pos[3];

    glGetUniformfv(shader[shaderID]->programID, location, pos);
    cam->Walk(distance);
    glUniform3f(location, cam->m_position.x, cam->m_position.y, cam->m_position.z);
}

void Engine::Turn(float angle, int shaderID)
{
    if (shaderID > maxNumShaders-1)
        return;

    glUseProgram(shader[shaderID]->programID);
    // get our camera position from compute shader
    int locationA = glGetUniformLocation(shader[shaderID]->programID, "camAt");
    int locationR = glGetUniformLocation(shader[shaderID]->programID, "camRight");
    int locationU = glGetUniformLocation(shader[shaderID]->programID, "camUp");
    GLfloat at[3], right[3], up[3];
    cam->Orient(angle, 0.0f);
   
    glGetUniformfv(shader[shaderID]->programID, locationA, at);
    glUniform3f(locationA, cam->m_forward.x, cam->m_forward.y, cam->m_forward.z);

    glGetUniformfv(shader[shaderID]->programID, locationR, right);
    glUniform3f(locationR, cam->m_right.x, cam->m_right.y, cam->m_right.z);

    glGetUniformfv(shader[shaderID]->programID, locationU, up);
    glUniform3f(locationU, cam->m_up.x, cam->m_up.y, cam->m_up.z);
}

void Engine::SetTime(float t, int shaderID)
{
    if (shaderID > maxNumShaders-1)
        return;

    glUseProgram(shader[shaderID]->programID);
    // get our camera position from compute shader
    int location = glGetUniformLocation(shader[shaderID]->programID, "t");

    glUniform1f(location, t);
}
