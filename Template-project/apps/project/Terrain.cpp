#include "Terrain.hpp"
#include "ModelLoader.hpp"
#include "Model.hpp"
#include "TerrainTexturePack.hpp"
#include <vector>
#include <math.h>
#include <glad/glad.h>

Terrain::Terrain(float x, float z, ModelLoader &loader, TerrainTexturePack &pack, TerrainTexture &blendMap)
{
    this->pack = pack;
    this->blendMap = blendMap;
    this->size = 20;
    this->vertexCount = 64;
    this->x = x * size;
    this->z = z * size;
    this->model = generateTerrain(loader);
}

Model Terrain::generateTerrain(ModelLoader loader)
{
    // returns the model of a flat terrain of side length size and vertexCount vertices
    int count = pow(vertexCount, 2);
    std::vector<GLfloat> vertices;
    vertices.resize(count * 3);
    std::vector<GLfloat> normals;
    normals.resize(count * 3);
    std::vector<GLfloat> uv;
    uv.resize(count * 2);
    std::vector<GLuint> indices;
    indices.resize(6 * pow(vertexCount - 1, 2));

    int pointer = 0;
    for (int i = 0; i < vertexCount; i++)
    {
        for (int j = 0; j < vertexCount; j++)
        {
            vertices[pointer * 3] = -(float)j / ((float)vertexCount - 1) * size;
            vertices[pointer * 3 + 1] = 0;
            vertices[pointer * 3 + 2] = -(float)i / ((float)vertexCount - 1) * size;
            normals[pointer * 3] = 0;
            normals[pointer * 3 + 1] = 1;
            normals[pointer * 3 + 2] = 0;
            uv[pointer * 2] = j / (float)(vertexCount - 1);
            uv[pointer * 2 + 1] = i / (float)(vertexCount - 1);
            pointer++;
        }
    }

    pointer = 0;
    for (int gz = 0; gz < vertexCount - 1; gz++)
    {
        for (int gx = 0; gx < vertexCount - 1; gx++)
        {
            int topLeft = (gz * vertexCount) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * vertexCount) + gx;
            int bottomRight = bottomLeft + 1;
            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }
    return loader.loadToVAO(vertices, indices, uv, normals);
}

float Terrain::getX()
{
    return x;
}

float Terrain::getZ()
{
    return z;
}

Model Terrain::getModel()
{
    return model;
}

TerrainTexture Terrain::getBlendMap()
{
    return blendMap;
}

TerrainTexturePack Terrain::getTexturePack()
{
    return pack;
}