#ifndef OBJECTSHADER_H
#define OBJECTSHADER_H

#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include <string>
#include <glad/glad.h>
#include "./glm/glm.hpp"
#include <vector>

class ObjectShader : public ShaderProgram
{
private:
    GLuint loc_transformationMatrix;
    GLuint loc_projectionMatrix;
    GLuint loc_viewMatrix;
    GLuint loc_texture;
    std::vector<GLuint> loc_lightPosition;
    std::vector<GLuint> loc_lightColor;
    std::vector<GLuint> loc_attenuation;
    GLuint loc_shineDamper;
    GLuint loc_reflectivity;
    GLuint loc_clipPlane;
    int maxLights;

protected:
    void bindAttribute(int attr, std::string varName);
    void bindAttributes();

public:
    ObjectShader() = default;
    ObjectShader(const std::string vsSource, const std::string fsSource);
    void loadTransformationMatrix(glm::mat4 matrix);
    void loadProjectionMatrix(glm::mat4 matrix);
    void loadViewMatrix(Camera &camera);
    void loadLights(std::vector<Light> &lights);
    void loadShineVars(float damper, float reflectivity);
    void getUniformLocations();
    void loadClipPlane(glm::vec4 plane);
};

#endif