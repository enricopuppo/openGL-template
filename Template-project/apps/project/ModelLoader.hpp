#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include "Model.hpp"

class ModelLoader
{
private:
    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
    std::vector<GLuint> textures;

    GLuint createVAO();
    void storeDataInAttrList(int attrNum, int attrSize, std::vector<GLfloat> &data);
    void unbindVAO();

public:
    void bindIndicesBuffer(std::vector<GLuint> &indices);
    GLuint loadTexture(const std::string path, GLenum type);
    Model loadToVAO(std::vector<GLfloat> &vertices, int nTris);
    Model loadToVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, std::vector<GLfloat> &uv, std::vector<GLfloat> &normals);
    void clean();
};

#endif