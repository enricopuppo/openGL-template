#ifndef TERRAIN_H
#define TERRAIN_H

#include "Model.hpp"
#include "TerrainTexture.hpp"
#include "TerrainTexturePack.hpp"
#include "ModelLoader.hpp"

class Terrain{
    private:
        float size;
        int vertexCount;
        float x;
        float z;
        Model model;
        TerrainTexturePack pack;
        TerrainTexture blendMap;
        Model generateTerrain(ModelLoader loader);

    public:
        Terrain(float x, float z, ModelLoader &loader, TerrainTexturePack &pack, TerrainTexture & blendMap);
        float getX();
        float getZ();
        Model getModel();
        TerrainTexturePack getTexturePack();
        TerrainTexture getBlendMap();
};

#endif