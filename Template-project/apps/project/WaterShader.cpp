#include "WaterShader.hpp"
#include "MatrixGenerator.hpp"

WaterShader::WaterShader(const std::string vsSource, const std::string fsSource) : ShaderProgram::ShaderProgram(vsSource, fsSource){}

void WaterShader::loadModelMatrix(glm::mat4 matrix){
    ShaderProgram::loadMatrix(loc_modelMatrix, matrix);
}

void WaterShader::loadProjectionMatrix(glm::mat4 matrix){
    ShaderProgram::loadMatrix(loc_projectionMatrix, matrix);
}

void WaterShader::loadViewMatrix(Camera &camera){
    if(camera.isFirstPersonViewEnabled()){
        MatrixGenerator mg = MatrixGenerator();
        ShaderProgram::loadMatrix(loc_viewMatrix, mg.createViewMatrix(camera));
    }
    else
        ShaderProgram::loadMatrix(loc_viewMatrix, camera.getViewMatrix());
    loadVector(loc_cameraPosition, camera.getPosition());
}

void WaterShader::loadMoveFactor(float factor){
    ShaderProgram::loadFloat(loc_moveFactor, factor);
}

void WaterShader::loadLight(Light &sun){
    ShaderProgram::loadVector(loc_lightColor, sun.getColor());
    ShaderProgram::loadVector(loc_lightPosition, sun.getPosition());
}

void WaterShader::getUniformLocations(){
    loc_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
    loc_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
    loc_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
    loc_reflectionTexture = ShaderProgram::getUniformLocation("reflectionTexture");
    loc_refractionTexture = ShaderProgram::getUniformLocation("refractionTexture");
    loc_dudvMap = ShaderProgram::getUniformLocation("dudvSampler");
    loc_moveFactor = ShaderProgram::getUniformLocation("moveFactor");
    loc_cameraPosition = ShaderProgram::getUniformLocation("cameraPosition");
    loc_normalMap = ShaderProgram::getUniformLocation("normalSampler");
    loc_lightColor = ShaderProgram::getUniformLocation("lightColor");
    loc_lightPosition = ShaderProgram::getUniformLocation("lightPosition");
}

void WaterShader::bindAttributes(){
    WaterShader::bindAttribute(0, "position");
}

void WaterShader::bindAttribute(int attr, std::string varName){
    ShaderProgram::bindAttribute(attr, varName);
}

void WaterShader::connectTextureUnits(){
    ShaderProgram::loadInt(loc_reflectionTexture, 0);
    ShaderProgram::loadInt(loc_refractionTexture, 1);
    ShaderProgram::loadInt(loc_dudvMap, 2);
    ShaderProgram::loadInt(loc_normalMap, 3);
}