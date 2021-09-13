#include "WaterTile.hpp"

WaterTile::WaterTile(float x, float y, float z, float tileSize){
    // stores the informations relative to a water tile
    this->x = x;
    this->y = y;
    this->z = z;
    this->tileSize = tileSize;
}

float WaterTile::getX(){
    return this->x;
}

float WaterTile::getY(){
    return this->y;
}

float WaterTile::getZ(){
    return this->z;
}

float WaterTile::getTileSize(){
    return this->tileSize;
}