#ifndef ENTITY_H
#define ENTITY_H

#include "./glm/glm.hpp"
#include "TexturedModel.hpp"

class Entity{
    private:
        TexturedModel model;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

    public:
        Entity() = default;
        Entity(TexturedModel model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale); 
        void editPosition(glm::vec3 deltas);
        void editRotation(glm::vec3 deltas);
        void editScale(glm::vec3 deltas);
        TexturedModel getModel();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();
};

#endif