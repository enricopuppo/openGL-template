#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "ShaderProgram.hpp"
#include "Camera.hpp"

class WaterShader : public ShaderProgram{
    private:
        GLuint loc_modelMatrix;
        GLuint loc_viewMatrix;
        GLuint loc_projectionMatrix;
        GLuint loc_reflectionTexture;
        GLuint loc_refractionTexture;
        GLuint loc_dudvMap;
        GLuint loc_moveFactor;
        GLuint loc_cameraPosition;
        GLuint loc_normalMap;
        GLuint loc_lightColor;
        GLuint loc_lightPosition;

    protected:
        void bindAttribute(int attr, std::string varName);
        void bindAttributes();

    public:
        WaterShader() = default;
        WaterShader(const std::string vsSource, const std::string fsSource);
        void getUniformLocations();
        void loadModelMatrix(glm::mat4 matrix);
        void loadProjectionMatrix(glm::mat4 matrix);
        void loadViewMatrix(Camera &camera);
        void loadMoveFactor(float factor);
        void loadLight(Light &sun);
        void connectTextureUnits();
};

#endif 