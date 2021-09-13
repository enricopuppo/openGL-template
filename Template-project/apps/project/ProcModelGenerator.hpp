#ifndef PROCMODELGEN_H
#define PROCMODELGEN_H

#include "Model.hpp"
#include "ModelLoader.hpp"
#include "Entity.hpp"
#include "Texture.hpp"
#include <vector>
#include <tuple>
#include <set>
#include <glad/glad.h>

class ProcModelGenerator
{
private:
    ModelLoader loader;
    std::set<std::tuple<float, float, float>> expand(std::vector<std::tuple<float, float, float>> &verts, int level);
    static bool tupleComp(std::tuple<float, float, float> a, std::tuple<float, float, float> b);
    std::vector<std::tuple<float, float, float>> setSort(std::set<std::tuple<float, float, float>> &verts_set);
    std::vector<std::tuple<float, float, float>> inflate(std::vector<std::tuple<float, float, float>> &verts);
    std::vector<std::tuple<int, int, int>> makeTris(std::vector<std::tuple<float, float, float>> &vertex_list);

public:
    ProcModelGenerator(ModelLoader &loader);
    Model genQuad();
    Model genCube();
    Model genCone(int res);
    Model genPool();
    Model genSphereArc(int level);
    std::vector<Entity> genSphere(int level, glm::vec3 position, float scale, Texture &texture);
    std::vector<Entity> genTree(int res, float positionX, float positionZ, float scale, Texture &textureFoliage, Texture &textureTrunk);
};

#endif