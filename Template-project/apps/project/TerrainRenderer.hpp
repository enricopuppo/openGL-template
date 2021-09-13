#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include "TerrainShader.hpp"
#include "Terrain.hpp"
#include "Entity.hpp"
#include "./glm/glm.hpp"
#include "./glm/ext.hpp"
#include "./glm/gtx/string_cast.hpp"
#include <vector>

class TerrainRenderer{
    private:
        TerrainShader shader;

        void prepareTerrain(Terrain terrain);
        void unbindTexturedModel();
        void loadModelMatrix(Terrain &terrain);
        void bindTextures(Terrain terrain);

    public:
        TerrainRenderer() = default;    
        TerrainRenderer(TerrainShader shader, glm::mat4 projectionMatrix);
        void render(std::vector<Terrain> terrains);
};

#endif