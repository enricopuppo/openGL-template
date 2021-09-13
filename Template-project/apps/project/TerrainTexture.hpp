#ifndef TERRAINTEXTURE_H
#define TERRAINTEXTURE_H

#include <glad/glad.h>

class TerrainTexture
{
private:
    GLuint textureID;

public:
    TerrainTexture() = default;
    TerrainTexture(GLuint textureID);
    GLuint getTextureID();
};

#endif