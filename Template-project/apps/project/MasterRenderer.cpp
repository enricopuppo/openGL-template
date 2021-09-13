#include "MasterRenderer.hpp"
#include "ObjectShader.hpp"
#include "EntityRenderer.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "TexturedModel.hpp"
#include "Terrain.hpp"
#include <tuple>
#include <vector>
#include "./glm/glm.hpp"
#include "./glm/ext.hpp"
#include "./glm/gtx/string_cast.hpp"

void MasterRenderer::setup(){
    // sets the clear color and clears the window
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int MasterRenderer::isModelIn(std::vector<std::tuple<TexturedModel, std::vector<Entity>>> &entities, TexturedModel &model){
    // cheks if the entities vector already contains the given model, and if so reurns the position
    for(size_t i=0; i<entities.size(); ++i){
        if(&std::get<0>(entities[i]) == &model)
            return i;
    }
    return -1;
}

MasterRenderer::MasterRenderer(const std::string vsSource, const std::string fsSource, const std::string terrainVsSource, const std::string terrainFsSource, float aspectRatio){
    this->aspectRatio = aspectRatio;
    createProjectionMatrix();
    shader = ObjectShader(vsSource, fsSource);
    shader.getUniformLocations();
    terrainShader = TerrainShader(terrainVsSource, terrainFsSource);
    terrainShader.getUniformLocations();
    renderer = EntityRenderer(shader, projectionMatrix);
    terrainRenderer = TerrainRenderer(terrainShader, projectionMatrix);
}

void MasterRenderer::render(std::vector<Light> &lights, Camera &camera, glm::vec4 clipPlane){
    // clears the screen and then renders all the elements, using each time the specific renderer
    setup();
    shader.start();
    shader.loadClipPlane(clipPlane);
    shader.loadLights(lights);
    shader.loadViewMatrix(camera);
    renderer.render(entities);
    shader.stop();
    terrainShader.start();
    terrainShader.loadLights(lights);
    terrainShader.loadViewMatrix(camera);
    terrainRenderer.render(terrains);
    terrainShader.stop();
    entities.clear();
    terrains.clear();
}

void MasterRenderer::processEntity(Entity &entity){
    // given an entity checks if its model is already present into the entities vector.
    // if not it appends to it a new tuple (model, vector of entities) having entity as the first element of the list
    // otherwise it simply pushes back the new entity to the corresponding list
    TexturedModel model = entity.getModel();
    int pos = isModelIn(entities, model);
    if(pos != -1)   // already existing batch
        std::get<1>(entities[pos]).push_back(entity);
    else{
        std::vector<Entity> newBatch = {entity};
        std::tuple<TexturedModel, std::vector<Entity>> newTuple = std::make_tuple(model, newBatch);
        entities.push_back(newTuple);
    }
}

void MasterRenderer::processTerrain(Terrain &terrain){
    // adds the terrain to the terrain list.
    // we do not have different types of terrain so a structure like the one above is not necessary
    terrains.push_back(terrain);
}

void MasterRenderer::createProjectionMatrix(){
    // builds the projection matrix using the field parameters
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void MasterRenderer::clean(){
    shader.clean();
    terrainShader.clean();
}

void MasterRenderer::renderScene(std::vector<Entity> &entities, std::vector<Terrain> &terrains, std::vector<Light> &lights, Camera &camera, Player &player, glm::vec4 clipPlane){
    // given all the elements to be rendered from the main function builds the two structures and 
    // depending on the current view adds or not the submarine
    for(auto terrain : terrains)
        processTerrain(terrain);
    for(auto entity : entities)
        processEntity(entity);
    if(!camera.isFirstPersonViewEnabled())
        processEntity(player);
    render(lights, camera, clipPlane);
}

glm::mat4 MasterRenderer::getProjectionMatrix(){
    return projectionMatrix;
}