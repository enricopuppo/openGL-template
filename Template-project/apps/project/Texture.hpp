#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
private:
    GLuint textureID;
    float shineDamp = 1.0;
    float reflectivity = 0.0;
    bool transparency = false;

public:
    Texture() = default;
    Texture(GLuint textureID);
    GLuint getTextureID();
    void setDamper(float damper);
    void setReflectivity(float reflectivity);
    float getDamper();
    float getReflectivity();
    void setTransparency(bool value);
    bool getTransparency();
};

#endif