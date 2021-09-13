#ifndef LIGHT_H
#define LIGHT_H

#include "./glm/glm.hpp"

class Light{
    private:
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 attenuation;
        glm::vec3 tint;
        float gain;

        void checkGain();

    public:
        Light() = default;
        Light(glm::vec3 position, glm::vec3 tint, float gain);
        Light(glm::vec3 position, glm::vec3 tint, float gain, glm::vec3 attenuation);
        glm::vec3 getPosition();
        glm::vec3 getColor();
        glm::vec3 getAttenuation();
        void editPosition(glm::vec3 deltas);
        void setAttenuation(glm::vec3 attenuation);
        void setGain(float gain);
};

#endif