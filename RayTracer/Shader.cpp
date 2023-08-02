#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    // open the vertex shader file
    std::ifstream vertexShaderfileStream(vertexShaderFilePath);
    const char* vertexShaderData;
    const char* fragmentShaderData;

    std::string strVertex;

    std::stringstream vertexShaderStream;

    if (!vertexShaderfileStream.is_open())
        std::cout << "failed to open " << vertexShaderFilePath << '\n';
    else
    {  
        vertexShaderStream << vertexShaderfileStream.rdbuf();
        strVertex = vertexShaderStream.str();
        vertexShaderData = strVertex.c_str();
    }

    vertexShaderfileStream.close();

    // do the same for fragment shader
    std::ifstream fragmentShaderFileStream(fragmentShaderFilePath);

    std::string strFragment;
    std::stringstream fragmentShaderStream;

    if (!fragmentShaderFileStream.is_open())
        std::cout << "failed to open " << fragmentShaderFilePath << '\n';
    else
    {
        fragmentShaderStream << fragmentShaderFileStream.rdbuf();
        strFragment = fragmentShaderStream.str();
        fragmentShaderData = strFragment.c_str();
    }

    fragmentShaderFileStream.close();

    unsigned int vertex, fragment;
    GLint success=-1;
    char log[512];

    // create vertex shader and compile it
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderData, NULL);
    glCompileShader(vertex);
    
    // check if compilation has no errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, log);
        std::cout << "ERROR: Vertex shader compilation failed\n" << log << std::endl;
        return;
    }

    // do the same for fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderData, NULL);
    glCompileShader(fragment);

    success = -1;

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, log);
        std::cout << "ERROR: Fragment shader compilation failed\n" << log << std::endl;
        return;
    }

    programID = glCreateProgram();

    if (programID == 0)
    {
        std::cout << "ERROR: Program could not be created" << std::endl;

    }

    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    GLint programLinkSuccess = -1;
    // check if it has been successful and write error message if not
    glGetProgramiv(programID, GL_LINK_STATUS, &programLinkSuccess);
    if (!programLinkSuccess)
    {
        glGetProgramInfoLog(programID, 512, NULL, log);
        std::cout << "ERROR: Program linking failed\n" << log << std::endl;
    }

   // we no longer need those, as they have been compiled and linked
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

Shader::Shader(const char* computeShaderFilePath)
{
    std::ifstream computeShaderfileStream(computeShaderFilePath);

    std::string strCompute;
    
    const char* computeShaderData;

    std::stringstream computeShaderStream;

    if (!computeShaderfileStream.is_open())
        std::cout << "failed to open " << computeShaderFilePath << '\n';
    else
    {
        computeShaderStream << computeShaderfileStream.rdbuf();
        strCompute = computeShaderStream.str();
        computeShaderData = strCompute.c_str();
    }

    computeShaderfileStream.close();
    char log[512];
    GLint success=-1;
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderData, NULL);
    glCompileShader(computeShader);


    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(computeShader, 512, NULL, log);
        std::cout << "ERROR: Compute shader compilation failed\n" << log << std::endl;
        return;
    }

    programID = glCreateProgram();

    if (programID == 0)
    {
        std::cout << "ERROR: Program could not be created" << std::endl;
    }

    glAttachShader(programID, computeShader);
    glLinkProgram(programID);
    GLint programLinkSuccess = -1;
    // check if it has been successful and write error message if not
    glGetProgramiv(programID, GL_LINK_STATUS, &programLinkSuccess);
    if (!programLinkSuccess)
    {
        glGetProgramInfoLog(programID, 512, NULL, log);
        std::cout << "ERROR: Program linking failed\n" << log << std::endl;
    }

    glDeleteShader(computeShader);
}
