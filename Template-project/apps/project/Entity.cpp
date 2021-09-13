#include "Entity.hpp"
#include "./glm/glm.hpp"
#include "TexturedModel.hpp"
#include <stdio.h>

Entity::Entity(TexturedModel model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    // gets the transformations needed for the model
    this->model = model;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

void Entity::editPosition(glm::vec3 deltas){
    // edits the position vector
    this->position[0] += deltas[0];
    this->position[1] += deltas[1];
    this->position[2] += deltas[2];
}

void Entity::editRotation(glm::vec3 deltas){
    // edits the rotation vector
    this->rotation[0] += deltas[0];
    this->rotation[1] += deltas[1];
    this->rotation[2] += deltas[2];
}

void Entity::editScale(glm::vec3 deltas){
    // edits the scale vector
    scale[0] += deltas[0];
    scale[1] += deltas[1];
    scale[2] += deltas[2];
}

TexturedModel Entity::getModel(){
    return model;
}

glm::vec3 Entity::getPosition(){
    return position;
}

glm::vec3 Entity::getRotation(){
    return rotation;
}

glm::vec3 Entity::getScale(){
    return scale;
}