#include "ShaderProgram.hpp"
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <glad/glad.h>
#include "./glm/glm.hpp"
#include "./glm/gtc/type_ptr.hpp"
using namespace std;

GLuint ShaderProgram::loadShader(const char *sourcePath, int type)
{
    // given a path to a shader program and a type (vertex/fragment) loads the source code.
    // creates and compiles the shader. If everything works returns the shader ID
    string shaderCode;
    ifstream shaderStream(sourcePath, ios::in);
    if (shaderStream.is_open())
    {
        stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }
    else
    {
        return 0;
    }

    GLuint shaderID = glCreateShader(type);
    const char *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    GLint res;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        printf("Cannot compile shader.\n");
        return -1;
    }
    return shaderID;
}

void ShaderProgram::bindAttribute(int attr, std::string varName)
{
    // given an attribute index and its string binds it to a program
    glBindAttribLocation(programID, attr, varName.c_str());
}

ShaderProgram::ShaderProgram(std::string vsSource, std::string fsSource)
{
    // given the two paths to the source codes of the shaders creates and links a program,
    // and binds to it its attributes
    vertexShaderID = loadShader(vsSource.c_str(), GL_VERTEX_SHADER);
    fragmentShaderID = loadShader(fsSource.c_str(), GL_FRAGMENT_SHADER);
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
}

GLuint ShaderProgram::getUniformLocation(std::string name)
{
    // given an uniform name returns the uniform location
    return glGetUniformLocation(programID, name.c_str());
}

void ShaderProgram::loadBool(GLuint loc, bool value)
{
    if (value)
        glUniform1f(loc, 1);
    else
        glUniform1f(loc, 0);
}

void ShaderProgram::loadFloat(GLuint loc, GLfloat value)
{
    glUniform1f(loc, value);
}

void ShaderProgram::loadInt(GLuint loc, GLint value)
{
    glUniform1i(loc, value);
}

void ShaderProgram::loadVector(GLuint loc, glm::vec3 vec)
{
    glUniform3f(loc, vec[0], vec[1], vec[2]);
}

void ShaderProgram::loadVector(GLuint loc, glm::vec4 vec)
{
    glUniform4f(loc, vec[0], vec[1], vec[2], vec[3]);
}

void ShaderProgram::loadMatrix(GLuint loc, glm::mat4 mat)
{
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::start()
{
    // loads the program
    glUseProgram(programID);
}

void ShaderProgram::stop()
{
    // releases the program
    glUseProgram(0);
}

void ShaderProgram::clean()
{
    // stops the current program if in use and deletes all its components
    stop();
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(programID);
}