#include "Quaternion.hpp"
#include <math.h>
#include <stdio.h>
#include "./glm/glm.hpp"
#include "./glm/gtx/string_cast.hpp"
#include <iostream>

// default constructor
Quaternion::Quaternion(){
    this->w = 1;
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->norm = 1;
}

// component constructor
Quaternion::Quaternion(float w, float x, float y, float z){
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
    normalize();
}

// vector-angle constructor
Quaternion::Quaternion(glm::vec3 v, float theta){
    v = glm::normalize(v);
    theta /= 2;
    this->w = cosf(theta);
    this->x = v[0] * sinf(theta);
    this->y = v[1] * sinf(theta);
    this->z = v[2] * sinf(theta);
}

// ||V||
float Quaternion::getMagnitude(){
    return sqrt(pow(this->w,2) + pow(this->x,2) + pow(this->y,2) + pow(this->z,2));
}

// computes the magnitude of the quaternion and normalizes it
void Quaternion::normalize(){
    this->norm = getMagnitude();
    this->w /= norm;
    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
}

// computes the sum of two quaternions
Quaternion Quaternion::operator + (const Quaternion &q){
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z); 
}

// computes the difference of two quaternions
Quaternion Quaternion::operator - (const Quaternion &q){
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z); 
}

// computes the product by scalar
Quaternion Quaternion::operator * (const float c){
    return Quaternion(w * c, x * c, y * c, z * c); 
}

// computes the sign inversion
Quaternion Quaternion::operator - (){
    return Quaternion(-w, -x, -y, -z);
}

// computes the product of two quaternions
Quaternion Quaternion::operator * (const Quaternion &q){
    float res_w = w * q.w - glm::dot(glm::vec3(x, y, z), glm::vec3(q.x, q.y, q.z));  // w1w2 - v1.v2
    glm::vec3 p1 = glm::vec3(x, y, z) * q.w;  // v1w2
    glm::vec3 p2 = w * glm::vec3(q.x, q.y, q.z);  // w1v2
    glm::vec3 p3 = glm::cross(glm::vec3(x, y, z), glm::vec3(q.x, q.y, q.z));    //v1 x v2
    glm::vec3 v = p1 + p2 + p3;
    return Quaternion(res_w, v[0], v[1], v[2]);
}

// generates the rotation matrix relative to the quaternion
glm::mat4 Quaternion::toMatrix4(){
    glm::mat4 res;
    res[0][0] = 1 - 2 * pow(y, 2) - 2 * pow(z, 2);
    res[0][1] = 2 * x * y - 2 * w * z;
    res[0][2] = 2 * x * z + 2 * w * y;
    res[0][3] = 0;
    res[1][0] = 2 * x * y + 2 * w * z;
    res[1][1] = 1 - 2 * pow(x, 2) - 2 * pow(z, 2);
    res[1][2] = 2 * y * z - 2 * w * x;
    res[1][3] = 0;
    res[2][0] = 2 * x * z - 2 * w * y;
    res[2][1] = 2 * y * z + 2 * w * x;
    res[2][2] = 1 - 2 * pow(x, 2) - 2 * pow(y, 2);
    res[2][3] = 0;
    res[3][0] = res[3][1] = res[3][2] = 0;
    res[3][3] = 1;
    return res;
}

Quaternion::Quaternion(glm::mat3 mat){
    this->w = std::sqrt(1.0 + mat[0][0] + mat[1][1] + mat[2][2]) / 2.0;
    this->x = (mat[2][1] - mat[1][2]) / (4.0 * w);
    this->y = (mat[0][2] - mat[2][0]) / (4.0 * w);
    this->z = (mat[1][0] - mat[0][1]) / (4.0 * w);
    normalize();
}

// returns the rotation angle
float Quaternion::getAngle(){
    return 2 * acosf(w);
}

// returns the rotation axis
glm::vec3 Quaternion::getAxis(){
    return glm::normalize(glm::vec3(x,y,z));
}

// returns a vector with the quaterion components
glm::vec4 Quaternion::getWxyz(){
    return glm::vec4(w, x, y, z);
}

// prints the quaternion's components
void Quaternion::print(){
    printf("[%f, (%f, %f, %f)]\n", this->w, this->x, this->y, this->z); // w in normalized radians
}