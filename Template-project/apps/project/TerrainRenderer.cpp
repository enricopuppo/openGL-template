#include "TerrainRenderer.hpp"
#include "Terrain.hpp"
#include "Entity.hpp"
#include "MatrixGenerator.hpp"
#include "./glm/glm.hpp"
#include "./glm/ext.hpp"
#include "./glm/gtx/string_cast.hpp"
#include <vector>

TerrainRenderer::TerrainRenderer(TerrainShader shader, glm::mat4 projectionMatrix){
    // loads the projectionMatrix  once for all
    this->shader = shader;
    shader.start();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.connectTextureUnits();
    shader.stop();
}

void TerrainRenderer::render(std::vector<Terrain> terrains){
    // renders all the terrain tiles
    for(auto terrain : terrains){
        glDisable(GL_CULL_FACE);
        prepareTerrain(terrain);
        loadModelMatrix(terrain);
        glDrawElements(GL_TRIANGLES, terrain.getModel().getVertexCount(), GL_UNSIGNED_INT, 0);   // render
        unbindTexturedModel();
    }
}

void TerrainRenderer::prepareTerrain(Terrain terrain){
    glBindVertexArray(terrain.getModel().getVaoID());    // loads the model's VAO
    glEnableVertexAttribArray(0);   // enables vertices attrib array
    glEnableVertexAttribArray(1);   // enables texture coords attrib array
    glEnableVertexAttribArray(2);   // enables normals attrib array
    bindTextures(terrain);
    shader.loadShineVars(1.0, 0.0);
}

void TerrainRenderer::bindTextures(Terrain terrain){
    // binds all the textures of the pack together with the blend map
    TerrainTexturePack pack = terrain.getTexturePack();
    glActiveTexture(GL_TEXTURE0);   // activates the second texture slot
    glBindTexture(GL_TEXTURE_2D, pack.getRTexture().getTextureID());    // binds the texture of the grass
    glActiveTexture(GL_TEXTURE1);   // activates the third texture slot
    glBindTexture(GL_TEXTURE_2D, pack.getGTexture().getTextureID());    // binds the texture of the plank
    glActiveTexture(GL_TEXTURE2);   // activates the fourth texture slot
    glBindTexture(GL_TEXTURE_2D, pack.getBTexture().getTextureID());    // binds the texture of the road
    glActiveTexture(GL_TEXTURE3);   // activates the fifth texture slot
    glBindTexture(GL_TEXTURE_2D, terrain.getBlendMap().getTextureID());    // binds the blend map
}

void TerrainRenderer::unbindTexturedModel(){
    // back to normal bindings
    glEnable(GL_CULL_FACE);
    glDisableVertexAttribArray(0);  
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(Terrain &terrain){
    MatrixGenerator mg = MatrixGenerator();
    glm::mat4 transformationMatrix = mg.createTransformationMatrix(glm::vec3(terrain.getX(), 0.0, terrain.getZ()), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));      // builds the transformation matrix for the model
    shader.loadTransformationMatrix(transformationMatrix);      // loads the matrix
}