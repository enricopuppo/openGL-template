#include "Model.hpp"

Model::Model(int vaoID, int vertexCount){
    this->vaoID = vaoID;
    this->vertexCount = vertexCount;
}

int Model::getVaoID(){
    return vaoID;
}

int Model::getVertexCount(){
    return vertexCount;
}