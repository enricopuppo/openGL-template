#include "ProcModelGenerator.hpp"
#include "Model.hpp"
#include "ModelLoader.hpp"
#include "Entity.hpp"
#include "TexturedModel.hpp"
#include "Texture.hpp"
#include <math.h>
#include <glad/glad.h>
#include <cstdio>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
using namespace std;

ProcModelGenerator::ProcModelGenerator(ModelLoader &loader)
{
    this->loader = loader;
}

std::set<std::tuple<float, float, float>> ProcModelGenerator::expand(std::vector<std::tuple<float, float, float>> &verts, int level)
{
    // check if the desired amount of levels is reached
    if (level == 0)
        return std::set<std::tuple<float, float, float>>(verts.begin(), verts.end());

    // compute the new midpoints using the input ones
    std::tuple<float, float, float> mid1 = std::make_tuple((std::get<0>(verts[0]) + std::get<0>(verts[1])) / 2, (std::get<1>(verts[0]) + std::get<1>(verts[1])) / 2, (std::get<2>(verts[0]) + std::get<2>(verts[1])) / 2);
    std::tuple<float, float, float> mid2 = std::make_tuple((std::get<0>(verts[0]) + std::get<0>(verts[2])) / 2, (std::get<1>(verts[0]) + std::get<1>(verts[2])) / 2, (std::get<2>(verts[0]) + std::get<2>(verts[2])) / 2);
    std::tuple<float, float, float> mid3 = std::make_tuple((std::get<0>(verts[1]) + std::get<0>(verts[2])) / 2, (std::get<1>(verts[1]) + std::get<1>(verts[2])) / 2, (std::get<2>(verts[1]) + std::get<2>(verts[2])) / 2);

    // new triangles setup
    std::vector<tuple<float, float, float>> triangle1 = {verts[0], mid1, mid2}; // triangolo alto
    std::vector<tuple<float, float, float>> triangle2 = {mid1, verts[1], mid3}; // triangolo basso destro
    std::vector<tuple<float, float, float>> triangle3 = {mid2, mid3, verts[2]}; // triangolo basso sinistro
    std::vector<tuple<float, float, float>> triangle4 = {mid3, mid2, mid1};     // triangolo centrale

    // recursive call to extract the new inner vertices
    std::set<std::tuple<float, float, float>> res1 = expand(triangle1, level - 1);
    std::set<std::tuple<float, float, float>> res2 = expand(triangle2, level - 1);
    std::set<std::tuple<float, float, float>> res3 = expand(triangle3, level - 1);
    std::set<std::tuple<float, float, float>> res4 = expand(triangle4, level - 1);

    // result assembly as the union of the obtained sets in order to get unique vertices
    std::set<tuple<float, float, float>> res({mid1});
    res.insert(res1.begin(), res1.end());
    res.insert({mid2});
    res.insert(res2.begin(), res2.end());
    res.insert({mid3});
    res.insert(res3.begin(), res3.end());
    res.insert(res4.begin(), res4.end());
    return res;
}

bool ProcModelGenerator::tupleComp(std::tuple<float, float, float> a, std::tuple<float, float, float> b)
{ // in this case we don't care of y
    if (std::get<2>(a) < std::get<2>(b))
        return true;
    if (std::get<2>(a) == std::get<2>(b))
        return (std::get<0>(a) < std::get<0>(b));
    return false;
}

std::vector<tuple<float, float, float>> ProcModelGenerator::setSort(std::set<std::tuple<float, float, float>> &verts_set)
{ // given the set returns an ordered vector
    std::vector<tuple<float, float, float>> verts_list(verts_set.begin(), verts_set.end());
    std::sort(verts_list.begin(), verts_list.end(), tupleComp);
    std::reverse(verts_list.begin(), verts_list.end());
    return verts_list;
}

std::vector<tuple<float, float, float>> ProcModelGenerator::inflate(std::vector<std::tuple<float, float, float>> &verts)
{
    std::vector<std::tuple<float, float, float>> verts_inflated;
    for (size_t i = 0; i < verts.size(); i++)
    {
        // extraction of the coordinates for each vertex
        float x = get<0>(verts[i]);
        float y = get<1>(verts[i]);
        float z = get<2>(verts[i]);
        // normalization to get a sphere wedge
        float norm = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        verts_inflated.push_back(make_tuple(x / norm, y / norm, z / norm));
    }
    return verts_inflated;
}

std::vector<std::tuple<int, int, int>> ProcModelGenerator::makeTris(std::vector<std::tuple<float, float, float>> &vertex_list)
{
    int level = 0;
    float old_z = 1;
    int first_of_level = 0;
    std::vector<tuple<int, int, int>> tris;
    for (size_t i = 0; i < vertex_list.size(); i++)
    {                                                       // for each vertex
        std::tuple<float, float, float> t = vertex_list[i]; // t is the tuple with the current vertex coordinates
        if (std::get<2>(t) != old_z)
        { // if z is different from the past one the new vertex is in the lower level
            old_z = std::get<2>(t);
            first_of_level = i; // we save the right most vertex index (the one on the level with the smaller index)
            level++;
            if (level > 1)
            { // mid-vertices start to appear from the second level
                for (int j = 0; j < level - 1; j++)
                {
                    // add the triangle with the central vertex and the corresponding two of the above level
                    std::tuple<int, int, int> new_tris = std::make_tuple<int, int, int>(first_of_level + j + 1, first_of_level + (j + 1) - level, first_of_level + j - level);
                    tris.push_back(new_tris);
                }
            }
        }
        if (std::get<2>(t) != 0.0)
        { // if the vertex is not in the last level
            // append a new triangle formed by it and the lower two
            std::tuple<int, int, int> new_tris = std::make_tuple<int, int, int>((int)i, (int)(i + level + 1), (int)(i + level + 2));
            tris.push_back(new_tris);
        }
    }
    return tris;
}

Model ProcModelGenerator::genQuad()
{
    // returns a square model composed by two triangles
    vector<GLfloat> vertices = {
        -0.5, 0.0, -0.5,
        -0.5, 0.0, 0.5,
        0.5, 0.0, 0.5,
        0.5, 0.0, -0.5};
    vector<GLfloat> normals = {
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0};
    vector<GLfloat> uv = {
        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0};
    vector<GLuint> indices = {
        0, 1, 3,
        3, 1, 2};
    return loader.loadToVAO(vertices, indices, uv, normals);
}

Model ProcModelGenerator::genCube()
{
    // returns a cube model
    vector<GLfloat> vertices = {
        -0.5, 0.5, -0.5,
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, 0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5};
    vector<GLfloat> normals = {
        -0.3, 0.3, -0.3,
        -0.3, -0.3, -0.3,
        0.3, -0.3, -0.3,
        0.3, 0.3, -0.3,
        -0.3, 0.3, 0.3,
        -0.3, -0.3, 0.3,
        0.3, -0.3, 0.3,
        0.3, 0.3, 0.3};
    vector<GLfloat> uv = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0};
    vector<GLuint> indices = {
        0, 2, 1, // back
        0, 3, 2,
        4, 5, 7, // front
        7, 5, 6,
        1, 2, 5, // bottom
        2, 6, 5,
        2, 3, 6, // right
        6, 3, 7,
        3, 0, 4, // top
        3, 4, 7,
        0, 1, 4, // left
        1, 5, 4};
    return loader.loadToVAO(vertices, indices, uv, normals);
}

Model ProcModelGenerator::genCone(int res)
{
    // returns a cone model with tunable resolution
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLfloat> uv;
    vector<GLuint> indices;
    vertices.resize(res * 2 * 3);
    normals.resize(res * 2 * 3);
    uv.resize(res * 2 * 2, 0.0);
    indices.resize(res * 2 * 3);
    const float k = 1.0 / sqrt(1.25);
    for (int i = 0; i < res; i++)
    {
        float a = 2 * 3.1415 * i / res;
        float s = sin(a);
        float c = cos(a);
        vertices[i * 3] = c;
        vertices[i * 3 + 1] = -1.0;
        vertices[i * 3 + 2] = s;
        normals[i * 3] = c * k;
        normals[i * 3 + 1] = 0.5 * k;
        normals[i * 3 + 2] = s * k;
    }
    for (int i = res; i < res * 2; i++)
    {
        float a = 2 * 3.1415 * i / res;
        float s = sin(a);
        float c = cos(a);
        vertices[i * 3] = 0.0;
        vertices[i * 3 + 1] = 1.0;
        vertices[i * 3 + 2] = 0.0;
        normals[i * 3] = c * k;
        normals[i * 3 + 1] = 0.5 * k;
        normals[i * 3 + 2] = s * k;
    }
    for (int i = 0; i < res; i++)
    {
        int j = (i + 1) % res;
        int index = i * 2;
        indices[index * 3] = i + res;
        indices[index * 3 + 1] = j;
        indices[index * 3 + 2] = i;
        indices[(index + 1) * 3] = j + res;
        indices[(index + 1) * 3 + 1] = j;
        indices[(index + 1) * 3 + 2] = i + res;
    }
    return loader.loadToVAO(vertices, indices, uv, normals);
}

Model ProcModelGenerator::genPool()
{
    // returns the pool model
    // the duplicate vertices are needed for a correct lighting and texture application
    vector<GLfloat> vertices = {
        -0.5, 0.5, -0.5,  // 0
        -0.5, -0.5, -0.5, // 1
        0.5, -0.5, -0.5,  // 2
        0.5, 0.5, -0.5,   // 3
        -0.5, 0.5, 0.5,   // 4
        -0.5, -0.5, 0.5,  // 5
        0.5, -0.5, 0.5,   // 6
        0.5, 0.5, 0.5,    // 7
        -0.5, 0.5, -0.5,  // 8  duplicate 0
        -0.5, -0.5, -0.5, // 9  duplicate 1
        0.5, -0.5, -0.5,  // 10 duplicate 2
        0.5, 0.5, -0.5,   // 11 duplicate 3
        -0.5, 0.5, 0.5,   // 12 duplicate 4
        -0.5, -0.5, 0.5,  // 13 duplicate 5
        0.5, -0.5, 0.5,   // 14 duplicate 6
        0.5, 0.5, 0.5,    // 15 duplicate 7
        -0.5, -0.5, -0.5, // 16 triplicate 1
        0.5, -0.5, -0.5,  // 17 triplicate 2
        -0.5, -0.5, 0.5,  // 18 triplicate 5
        0.5, -0.5, 0.5,   // 19 triplicate 6
    };
    vector<GLfloat> normals = {
        // order left, top, down
        -1.0, 0.0, 0.0, // 0
        -1.0, 0.0, 0.0, // 1
        0.0, 0.0, -1.0, // 2
        0.0, 0.0, -1.0, // 3
        -1.0, 0.0, 0.0, // 4
        -1.0, 0.0, 0.0, // 5
        1.0, 0.0, 0.0,  // 6
        1.0, 0.0, 0.0,  // 7
        0.0, 0.0, -1.0, // 8  duplicate 0
        0.0, 0.0, -1.0, // 9  duplicate 1
        1.0, 0.0, 0.0,  // 10 duplicate 2
        1.0, 0.0, 0.0,  // 11 duplicate 3
        0.0, 0.0, 1.0,  // 12 duplicate 4
        0.0, -1.0, 0.0, // 13 duplicate 5
        0.0, -1.0, 0.0, // 14 duplicate 6
        0.0, 0.0, 1.0,  // 15 duplicate 7
        0.0, -1.0, 0.0, // 16 triplicate 1
        0.0, -1.0, 0.0, // 17 triplicate 2
        0.0, 0.0, 1.0,  // 18 triplicate 5
        0.0, 0.0, 1.0   // 19 triplicate 6
    };
    vector<GLfloat> uv = {
        0.0, 1.0, // 0 for left face
        0.0, 0.0, // 1 for left face
        0.0, 0.0, // 2 for back face
        0.0, 1.0, // 3 for back face
        1.0, 1.0, // 4 for left face
        1.0, 0.0, // 5 for left face
        0.0, 0.0, // 6 for right face
        0.0, 1.0, // 7 for right face
        1.0, 1.0, // 8 duplicate 0 for back face
        1.0, 0.0, // 9 duplicate 1 for back face
        1.0, 0.0, // 10 duplicate 2 for right face
        1.0, 1.0, // 11 duplicate 3 for right face
        0.0, 1.0, // 12 duplicate 4 for front face
        0.0, 1.0, // 13 duplicate 5 for bottom face
        1.0, 1.0, // 14 duplicate 6 for bottom face
        1.0, 1.0, // 15 duplicate 7 for front face
        0.0, 0.0, // 16 triplicate 1 for bottom face
        1.0, 0.0, // 17 triplicate 2 for bottom face
        0.0, 0.0, // 18 triplicate 5 for front face
        1.0, 0.0  // 19 triplicate 6 for front face

    };
    vector<GLuint> indices = {
        2, 8, 9, // back
        2, 3, 8,
        12, 19, 18, // front
        12, 15, 19,
        16, 14, 17, // bottom
        16, 13, 14,
        10, 7, 11, // right
        10, 6, 7,
        1, 0, 4, // left
        1, 4, 5};
    return loader.loadToVAO(vertices, indices, uv, normals);
}

Model ProcModelGenerator::genSphereArc(int level)
{
    // returns the model of 1/8 of a sphere
    // create a vector with the initial vertices
    std::vector<std::tuple<float, float, float>> vertices = {std::make_tuple(0.0, 0.0, 1.0), std::make_tuple(1.0, -1.0, 0.0), std::make_tuple(-1.0, -1.0, 0.0)};
    // call the expand function that returns a new vertices vector with the new vertices found at midpoints between given ones for the fixed number of levels
    std::set<std::tuple<float, float, float>> vertices_exp = expand(vertices, level);
    // generate a vector of vertices starting from the set and sorts them by z and then x coord
    std::vector<std::tuple<float, float, float>> vertices_list = setSort(vertices_exp);
    // based on the vertices vector returns a vector with the triplets of the vertices indices making a triangle
    std::vector<std::tuple<int, int, int>> triangles = makeTris(vertices_list);
    // normalizes the distance of the vertices from (0,0,0) in order to simulate points on a sphere
    std::vector<std::tuple<float, float, float>> vertices_inflated = inflate(vertices_list);

    // allocate the final arrays
    int nVerts = vertices_inflated.size();
    int nTris = triangles.size();
    vector<GLfloat> verticesFinal;
    verticesFinal.resize(nVerts * 3);
    vector<GLfloat> uv(nVerts * 2, 0.0);
    vector<GLuint> indices;
    indices.resize(nTris * 3);

    // translates the computed elements in the required vertices+tris form
    for (int i = 0; i < nVerts; i++)
    {
        float x = std::get<0>(vertices_inflated[i]);
        float y = std::get<1>(vertices_inflated[i]);
        float z = std::get<2>(vertices_inflated[i]);
        verticesFinal[i * 3] = x;
        verticesFinal[i * 3 + 1] = y;
        verticesFinal[i * 3 + 2] = z;
    }
    for (int i = 0; i < nTris; ++i)
    {
        int a = std::get<0>(triangles[i]);
        int b = std::get<1>(triangles[i]);
        int c = std::get<2>(triangles[i]);
        indices[i * 3] = c;
        indices[i * 3 + 1] = b;
        indices[i * 3 + 2] = a;
    }
    return loader.loadToVAO(verticesFinal, indices, uv, verticesFinal);
}

std::vector<Entity> ProcModelGenerator::genSphere(int level, glm::vec3 position, float scale, Texture &texture)
{
    // given the characteristics returns a vector of sphere eights arranged to form a whole sphere
    std::vector<Entity> arcs;
    for (int i = 0; i < 4; ++i)
    {
        Model model = genSphereArc(level);
        arcs.push_back(Entity(TexturedModel(model, texture), position, glm::vec3(0.0, 0.0, 90.0 * i), glm::vec3(scale, scale, scale)));
    }
    for (int i = 4; i < 8; ++i)
    {
        Model model = genSphereArc(level);
        arcs.push_back(Entity(TexturedModel(model, texture), position, glm::vec3(0.0, 180.0, 90.0 * i), glm::vec3(scale, scale, scale)));
    }
    return arcs;
}

std::vector<Entity> ProcModelGenerator::genTree(int res, float positionX, float positionZ, float scale, Texture &textureFoliage, Texture &textureTrunk)
{
    // given the characteristics returns a vector of cones arranged to form a pine tree
    std::vector<Entity> parts;
    Model cone = genCone(res);
    parts.push_back(Entity(TexturedModel(cone, textureFoliage), glm::vec3(positionX, 2.2 * scale, positionZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.5, 1.0) * scale));
    parts.push_back(Entity(TexturedModel(cone, textureFoliage), glm::vec3(positionX, 0.7 * scale, positionZ), glm::vec3(180.0, 0.0, 0.0), glm::vec3(1.0, 0.01, 1.0) * scale));
    parts.push_back(Entity(TexturedModel(cone, textureTrunk), glm::vec3(positionX, 0.5 * scale, positionZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5) * scale));
    return parts;
}