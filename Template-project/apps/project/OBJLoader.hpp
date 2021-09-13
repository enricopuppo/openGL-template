#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "ModelLoader.hpp"
#include "Model.hpp"
#include <string>
#include <vector>
#include "./glm/glm.hpp"

class OBJLoader{
    private:
        std::vector<std::string> lineToTokens(std::string line, std::string separator);
        void processVertex(std::vector<std::string> &vertexData, std::vector<GLuint> &indices, std::vector<glm::vec2> &fileUVs, std::vector<glm::vec3> &fileNormals, std::vector<GLfloat> &uv, std::vector<GLfloat> &normals);

    public:
        Model loadObjModel(std::string path, ModelLoader loader);
};

#endif