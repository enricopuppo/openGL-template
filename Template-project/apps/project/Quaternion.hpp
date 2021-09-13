#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>
#include "./glm/glm.hpp"
#include "./glm/gtx/string_cast.hpp"
#include <iostream>

class Quaternion{
    private:
        float w;
        float x;
        float y;
        float z;
        float norm;
    
    public:
        Quaternion();
        Quaternion(float w, float x, float y, float z);
        Quaternion(glm::vec3 v, float theta);
        Quaternion(glm::mat3 mat);
        float getMagnitude();
        void normalize();
        glm::mat4 toMatrix4();
        Quaternion fromMatrix(glm::mat3 matrix);
        float getAngle();
        glm::vec3 getAxis();
        glm::vec4 getWxyz();
        void print();
        void clamp(glm::vec3 lowThresh, glm::vec3 highThresh);
        // operators
        Quaternion operator + (const Quaternion &q);
        Quaternion operator - (const Quaternion &q);
        Quaternion operator - ();
        Quaternion operator * (const float s);
        Quaternion operator * (const Quaternion &q);
};
#endif