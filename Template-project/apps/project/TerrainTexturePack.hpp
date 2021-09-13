#ifndef TERRAINTEXTUREPACK_H
#define TERRAINTEXTUREPACK_H

#include <glad/glad.h>
#include "TerrainTexture.hpp"

class TerrainTexturePack
{
private:
    TerrainTexture rTexture;
    TerrainTexture gTexture;
    TerrainTexture bTexture;

public:
    TerrainTexturePack() = default;
    TerrainTexturePack(TerrainTexture &r, TerrainTexture &g, TerrainTexture &b);
    TerrainTexture getRTexture();
    TerrainTexture getGTexture();
    TerrainTexture getBTexture();
};

#endif