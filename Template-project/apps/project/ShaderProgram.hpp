#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <glad/glad.h>
#include "./glm/glm.hpp"

// general class for a shader program. Each program has to be defined as an instance
// of a sub-class that implements the bindAttribute and bindAttributes methods, defining all the needed attributes
class ShaderProgram
{
private:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    GLuint loadShader(const char *source_path, int type);

protected:
    GLuint getUniformLocation(std::string name);
    void loadBool(GLuint loc, bool value);
    void loadFloat(GLuint loc, GLfloat value);
    void loadInt(GLuint loc, GLint value);
    void loadVector(GLuint loc, glm::vec3 vec);
    void loadVector(GLuint loc, glm::vec4 vec);
    void loadMatrix(GLuint loc, glm::mat4 mat);
    virtual void bindAttribute(int attr, std::string varName);
    virtual void bindAttributes(){};

public:
    ShaderProgram() = default;
    ShaderProgram(std::string vsSource, std::string fsSource);
    virtual void getUniformLocations() = 0;
    void start();
    void stop();
    void clean();
};

#endif