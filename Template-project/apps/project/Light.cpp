#include "Light.hpp"
#include "./glm/glm.hpp"
#include <stdio.h>

Light::Light(glm::vec3 position, glm::vec3 tint, float gain){
    this->position = position;
    this->tint = tint;  // color tone of the light
    this->attenuation = glm::vec3(1.0, 0.0, 0.0);
    this->gain = gain;  // brightness of the light
    checkGain();
    this->color = tint * gain;  // final color used by the shaders
}

Light::Light(glm::vec3 position, glm::vec3 tint, float gain, glm::vec3 attenuation){
    this->position = position;
    this->tint = tint;
    this->attenuation = attenuation;
    this->gain = gain;
    checkGain();
    this->color = tint * gain;
}

glm::vec3 Light::getPosition(){
    return position;
}

glm::vec3 Light::getColor(){
    return color;
}

glm::vec3 Light::getAttenuation(){
    return attenuation;
}

void Light::setAttenuation(glm::vec3 attenuation){
    this->attenuation = attenuation;
}

void Light::setGain(float gain){
    this->gain = gain;
    checkGain();
    color = tint * gain;
}

void Light::editPosition(glm::vec3 deltas){
    this->position[0] += deltas[0];
    this->position[1] += deltas[1];
    this->position[2] += deltas[2];
}

void Light::checkGain(){
    if(gain > 1.0)
        gain = 1.0;
    else if(gain < 0.0)
        gain = 0.0;
}