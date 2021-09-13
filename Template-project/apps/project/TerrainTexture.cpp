#include "TerrainTexture.hpp"
#include <glad/glad.h>

TerrainTexture::TerrainTexture(GLuint textureID)
{
    this->textureID = textureID;
}

GLuint TerrainTexture::getTextureID()
{
    return textureID;
}