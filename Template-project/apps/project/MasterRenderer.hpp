#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include "ObjectShader.hpp"
#include "EntityRenderer.hpp"
#include "TerrainRenderer.hpp"
#include "TerrainShader.hpp"
#include "Terrain.hpp"
#include "Entity.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include <vector>
#include <tuple>

class MasterRenderer{
    private:
        ObjectShader shader;
        TerrainShader terrainShader;
        EntityRenderer renderer;
        TerrainRenderer terrainRenderer;
        glm::mat4 projectionMatrix;
        const float fov = 50.0f;
        const float nearPlane = 0.1f;
        const float farPlane = 1000.0f;
        float aspectRatio;
        std::vector<std::tuple<TexturedModel, std::vector<Entity>>> entities;
        std::vector<Terrain> terrains;

        int isModelIn(std::vector<std::tuple<TexturedModel, std::vector<Entity>>> &entities, TexturedModel &model);
        void createProjectionMatrix();

    public:
        MasterRenderer() = default;
        MasterRenderer(const std::string vsSource, const std::string fsSource, const std::string terrainVsSource, const std::string terrainFsSource, float aspectRatio);
        void setup();
        void render(std::vector<Light> &ambient, Camera &camera, glm::vec4 clipPlane);
        void processEntity(Entity &entity);
        void processTerrain(Terrain &terrain);
        void renderScene(std::vector<Entity> &entities, std::vector<Terrain> &terrains, std::vector<Light> &lights, Camera &camera, Player &player, glm::vec4 clipPlane);
        void clean();
        glm::mat4 getProjectionMatrix();
};

#endif