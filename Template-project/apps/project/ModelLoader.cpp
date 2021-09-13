#define STB_IMAGE_IMPLEMENTATION

#include "ModelLoader.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>
#include "stb_image.h"

GLuint ModelLoader::createVAO()
{
    // creates and returns a fresh vertex array object
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    VAOs.push_back(vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

void ModelLoader::storeDataInAttrList(int attrNum, int attrSize, std::vector<GLfloat> &data)
{
    // given an attribute number, a size and an array of data
    // creates a new vertex buffer object for a bound VAO and buffers the array.
    // attrNum is the index of the VBO in the VAO and attrSize tells how many elements are
    // specified per-vertex (i.e. 3 for positions, 2 for uv, ...)
    GLuint vboID;
    glGenBuffers(1, &vboID);
    VBOs.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attrNum, attrSize, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelLoader::unbindVAO()
{
    // resets the standard VAO
    glBindVertexArray(0);
}

void ModelLoader::bindIndicesBuffer(std::vector<GLuint> &indices)
{
    // generates a buffer for the triangle indices and transfers them
    GLuint vboID;
    glGenBuffers(1, &vboID);
    VBOs.push_back(vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

GLuint ModelLoader::loadTexture(const std::string path, GLenum type)
{
    // given an image path generates a new texture, sets some options and loads the image in it
    stbi_set_flip_vertically_on_load(true);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load
    int width, height, nChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
    if (data)
    {
        if (type == GL_RGBA)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else if (type == GL_RGB)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            printf("Wrong texture type!\n");
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -2.0); // quality degradation factor for mipmaps
    }
    else
        printf("Failed to load texture\n");
    stbi_image_free(data);
    textures.push_back(textureID);
    return textureID;
}

Model ModelLoader::loadToVAO(std::vector<GLfloat> &vertices, int nTris)
{
    int vaoID = createVAO();
    storeDataInAttrList(0, 2, vertices);
    unbindVAO();
    return Model(vaoID, vertices.size());
}

Model ModelLoader::loadToVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, std::vector<GLfloat> &uv, std::vector<GLfloat> &normals)
{
    // given the arrays of vertices, indices and uv coordinates creates a new VAO, loads all the data, restores
    // the default VAO and returns a new Model object
    int vaoID = createVAO();
    bindIndicesBuffer(indices);
    storeDataInAttrList(0, 3, vertices);
    storeDataInAttrList(1, 2, uv);
    storeDataInAttrList(2, 3, normals);
    unbindVAO();
    return Model(vaoID, indices.size());
}

void ModelLoader::clean()
{
    // using the VAO/VBO/texture arrays deletes everything
    for (size_t i = 0; i < VAOs.size(); ++i)
        glDeleteVertexArrays(1, &VAOs[i]);
    for (size_t i = 0; i < VBOs.size(); ++i)
        glDeleteBuffers(1, &VBOs[i]);
    for (size_t i = 0; i < textures.size(); ++i)
        glDeleteTextures(1, &textures[i]);
}