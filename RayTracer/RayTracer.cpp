// RayTracer.cpp 

#include <iostream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine.h"

int main()
{   
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(512, 512, "Ray Tracer", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
 
    Engine engine;
    double lastTime = 0.0, deltaTime = 0.0;
    float elapsedTime = 0.0;
    int renderer = 1;
    float fps = 0.0f;

    while (!glfwWindowShouldClose(window))
    {     
        double currentTime = glfwGetTime(); 
        
        deltaTime = currentTime - lastTime;

        lastTime = currentTime;    
        elapsedTime += deltaTime;

        engine.SetTime(elapsedTime, renderer);
   
        fps = 1.0f / deltaTime;

        if (renderer == 1)
            engine.RenderParallel(window);
        else if(renderer == 2)
            engine.RenderNormal(window);
       
        glfwPollEvents();

        //// move forward or back if user presses W or S and turn if they press A or D
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            engine.Move(deltaTime*10, renderer);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            engine.Move(-deltaTime * 10, renderer);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            engine.Turn(deltaTime * 100, renderer);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            engine.Turn(-deltaTime * 100, renderer);

        // change renderer if user presses 1 or 2
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            if (renderer != 1)
            {
                std::cout << "Current renderer: Parallel" << std::endl;
                renderer = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            if (renderer != 2)
            {
                std::cout << "Current renderer: Normal" << std::endl;
                renderer = 2;
            }
        }
    }

    if (renderer == 1)
        std::cout << "Average Frames per second for parallel ray tracer: " << fps << std::endl;
    else if (renderer == 2)
        std::cout << "Average Frames per second for normal ray tracer: " << fps << std::endl;
    
    glfwTerminate();
    return 0;
}
  

