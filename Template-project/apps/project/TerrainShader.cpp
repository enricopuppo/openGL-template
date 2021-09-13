#include "TerrainShader.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "MatrixGenerator.hpp"
#include <string>
#include "./glm/glm.hpp"

TerrainShader::TerrainShader(const std::string vsSource, const std::string fsSource) : ShaderProgram::ShaderProgram(vsSource, fsSource){
    this->maxLights = 4;
}

void TerrainShader::bindAttribute(int attr, std::string varName){
    ShaderProgram::bindAttribute(attr, varName);
}

void TerrainShader::bindAttributes(){
    TerrainShader::bindAttribute(0, "position");
    TerrainShader::bindAttribute(1, "textureCoords");
    TerrainShader::bindAttribute(2, "normal");
}

void TerrainShader::getUniformLocations(){
    loc_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
    loc_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
    loc_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
    loc_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
    loc_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
    loc_rTexture = ShaderProgram::getUniformLocation("rSampler");
    loc_gTexture = ShaderProgram::getUniformLocation("gSampler");
    loc_bTexture = ShaderProgram::getUniformLocation("bSampler");
    loc_blendMap = ShaderProgram::getUniformLocation("blendMapSampler");
    for(int i=0; i<maxLights; i++){
        loc_lightPosition.push_back(ShaderProgram::getUniformLocation("lightPosition["  + std::to_string(i) + "]"));
        loc_lightColor.push_back(ShaderProgram::getUniformLocation("lightColor[" + std::to_string(i) + "]"));
        loc_attenuation.push_back(ShaderProgram::getUniformLocation("attenuation[" + std::to_string(i) + "]"));
    }
}

void TerrainShader::connectTextureUnits(){
    ShaderProgram::loadInt(loc_rTexture, 0);
    ShaderProgram::loadInt(loc_gTexture, 1);
    ShaderProgram::loadInt(loc_bTexture, 2);
    ShaderProgram::loadInt(loc_blendMap, 3);
}

void TerrainShader::loadTransformationMatrix(glm::mat4 matrix){
    ShaderProgram::loadMatrix(loc_transformationMatrix, matrix);
}

void TerrainShader::loadProjectionMatrix(glm::mat4 matrix){
    ShaderProgram::loadMatrix(loc_projectionMatrix, matrix);
}

void TerrainShader::loadViewMatrix(Camera &camera){
    if(camera.isFirstPersonViewEnabled()){
        MatrixGenerator mg = MatrixGenerator();
        ShaderProgram::loadMatrix(loc_viewMatrix, mg.createViewMatrix(camera));
    }
    else
        ShaderProgram::loadMatrix(loc_viewMatrix, camera.getViewMatrix());
}

void TerrainShader::loadLights(std::vector<Light> &lights){
    for(int i=0; i<maxLights; ++i){     // accepts max 4 lights
        if((size_t)i<lights.size()){
            loadVector(loc_lightPosition[i], lights[i].getPosition());
            loadVector(loc_lightColor[i], lights[i].getColor());
            loadVector(loc_attenuation[i], lights[i].getAttenuation());
       }
       else{    // if less are given we add some dummy input to fill the array
            loadVector(loc_lightPosition[i], glm::vec3(0.0, 0.0, 0.0));
            loadVector(loc_lightColor[i], glm::vec3(0.0, 0.0, 0.0));
            loadVector(loc_attenuation[i], glm::vec3(1.0, 0.0, 0.0));
       }
    }
}

void TerrainShader::loadShineVars(float damper, float reflectivity){
    ShaderProgram::loadFloat(loc_shineDamper, damper);
    ShaderProgram::loadFloat(loc_reflectivity, reflectivity);
}