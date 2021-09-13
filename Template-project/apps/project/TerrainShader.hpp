#ifndef TERRAINSHADER_H
#define TERRAINSHADER_H

#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include <string>
#include <glad/glad.h>
#include "./glm/glm.hpp"
#include <vector>

class TerrainShader : public ShaderProgram
{
private:
    GLuint loc_transformationMatrix;
    GLuint loc_projectionMatrix;
    GLuint loc_viewMatrix;
    std::vector<GLuint> loc_lightPosition;
    std::vector<GLuint> loc_lightColor;
    std::vector<GLuint> loc_attenuation;
    GLuint loc_shineDamper;
    GLuint loc_reflectivity;
    GLuint loc_rTexture;
    GLuint loc_gTexture;
    GLuint loc_bTexture;
    GLuint loc_blendMap;
    int maxLights;

protected:
    void bindAttribute(int attr, std::string varName);
    void bindAttributes();

public:
    TerrainShader() = default;
    TerrainShader(const std::string vsSource, const std::string fsSource);
    void loadTransformationMatrix(glm::mat4 matrix);
    void loadProjectionMatrix(glm::mat4 matrix);
    void loadViewMatrix(Camera &camera);
    void loadLights(std::vector<Light> &lights);
    void loadShineVars(float damper, float reflectivity);
    void getUniformLocations();
    void connectTextureUnits();
};

#endif