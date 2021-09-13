#ifndef MATRIXGENERATOR_H
#define MATRIXGENERATOR_H

#include "./glm/glm.hpp"
#include "Camera.hpp"

class MatrixGenerator{
    public:
        glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
        glm::mat4 createViewMatrix(Camera &camera);
};

#endif