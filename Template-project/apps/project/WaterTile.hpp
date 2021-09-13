#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile{
    private:
        float x;
        float y;
        float z;
        float tileSize = 1.0;

    public:
        WaterTile() = default;
        WaterTile(float x, float y, float z, float tileSize);
        float getX();
        float getY();
        float getZ();
        float getTileSize();
};

#endif