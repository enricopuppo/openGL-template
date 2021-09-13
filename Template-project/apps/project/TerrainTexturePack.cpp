#include "TerrainTexturePack.hpp"
#include "TerrainTexture.hpp"

TerrainTexturePack::TerrainTexturePack(TerrainTexture &r, TerrainTexture &g, TerrainTexture &b){
    // stores the three textures that will be used to color the terrain
    this->rTexture = r;
    this->gTexture = g;
    this->bTexture = b;
}

TerrainTexture TerrainTexturePack::getRTexture(){
    return this->rTexture;
}

TerrainTexture TerrainTexturePack::getGTexture(){
    return this->gTexture;
}

TerrainTexture TerrainTexturePack::getBTexture(){
    return this->bTexture;
}