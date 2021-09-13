#include "MatrixGenerator.hpp"
#include "Camera.hpp"
#include "./glm/glm.hpp"
#include "./glm/ext.hpp"
#include "./glm/geometric.hpp"
#include "./glm/gtc/matrix_transform.hpp"
#include "./glm/gtx/string_cast.hpp"
#include <iostream>

glm::mat4 MatrixGenerator::createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale){
    // given the transformation vectors generates the generic transformation matrix
    glm::mat4 matrix = glm::mat4(1.0);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, glm::radians(rotation[0]), glm::vec3(1.0, 0.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(rotation[1]), glm::vec3(0.0, 1.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(rotation[2]), glm::vec3(0.0, 0.0, 1.0));
    matrix = glm::scale(matrix, scale);
    return matrix;
}

glm::mat4 MatrixGenerator::createViewMatrix(Camera &camera){
    // given the camera outputs the view matrix
    glm::mat4 matrix = glm::mat4(1.0);
    matrix = glm::rotate(matrix, glm::radians(camera.getPitch()), glm::vec3(1.0, 0.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(camera.getYaw()), glm::vec3(0.0, 1.0, 0.0));
    matrix = glm::rotate(matrix, glm::radians(camera.getRoll()), glm::vec3(0.0, 0.0, 1.0));
    matrix = glm::translate(matrix, -camera.getPosition());
    return matrix;
}