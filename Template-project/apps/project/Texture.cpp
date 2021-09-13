#include "Texture.hpp"
#include <glad/glad.h>

Texture::Texture(GLuint textureID)
{
    this->textureID = textureID;
}

GLuint Texture::getTextureID()
{
    return textureID;
}

float Texture::getDamper()
{
    return shineDamp;
}

float Texture::getReflectivity()
{
    return reflectivity;
}

void Texture::setDamper(float damper)
{
    this->shineDamp = damper;
}

void Texture::setReflectivity(float reflectivity)
{
    this->reflectivity = reflectivity;
}

void Texture::setTransparency(bool value)
{
    this->transparency = value;
}

bool Texture::getTransparency()
{
    return transparency;
}