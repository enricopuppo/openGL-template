#include "EntityRenderer.hpp"
#include "Model.hpp"
#include "TexturedModel.hpp"
#include "Entity.hpp"
#include "ObjectShader.hpp"
#include "MatrixGenerator.hpp"
#include <glad/glad.h>
#include <stdio.h>
#include "./glm/glm.hpp"
#include "./glm/ext.hpp"
#include "./glm/gtx/string_cast.hpp"
#include <iostream>
#include <tuple>
#include <vector>

EntityRenderer::EntityRenderer(ObjectShader &shader, glm::mat4 projectionMatrix)
{
    // given the shader stores the projection matrix once for all
    this->shader = shader;
    shader.start();
    shader.loadProjectionMatrix(projectionMatrix);
    shader.stop();
}

void EntityRenderer::render(std::vector<std::tuple<TexturedModel, std::vector<Entity>>> &entities)
{
    // renders efficiently all the entites of the scene
    // for each type of model the assets are loaded one time and they are used for each instance
    for (auto model : entities)
    {
        prepareTexturedModel(std::get<0>(model));
        std::vector<Entity> models = std::get<1>(model);
        for (size_t i = 0; i < models.size(); ++i)
        {
            prepareInstance(models[i]);
            GLenum err;
            glDrawElements(GL_TRIANGLES, models[i].getModel().getModel().getVertexCount(), GL_UNSIGNED_INT, 0);
            while ((err = glGetError()) != GL_NO_ERROR)
                printf("opengl error %d \n", err);
        }
        unbindTexturedModel();
    }
}

void EntityRenderer::prepareTexturedModel(TexturedModel model)
{
    glBindVertexArray(model.getModel().getVaoID()); // loads the model's VAO
    glEnableVertexAttribArray(0);                   // enables vertices attrib array
    glEnableVertexAttribArray(1);                   // enables texture coords attrib array
    glEnableVertexAttribArray(2);                   // enables normals attrib array

    Texture texture = model.getTexture();
    if (texture.getTransparency()) // disables culling to avoid artifacts on objects with transparent parts (plants)
        disableCulling();
    else
        enableCulling();
    shader.loadShineVars(texture.getDamper(), texture.getReflectivity()); // extracts the shine values for the texture
    glActiveTexture(GL_TEXTURE0);                                         // activates the first texture slot
    glBindTexture(GL_TEXTURE_2D, model.getTexture().getTextureID());      // binds the texture of the model
}

void EntityRenderer::unbindTexturedModel()
{
    // back to normal bindings
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(Entity &entity)
{
    // creates the transformation matrix for the current entity
    // and loads it for the shader
    MatrixGenerator mg = MatrixGenerator();
    glm::mat4 transformationMatrix = mg.createTransformationMatrix(entity.getPosition(), entity.getRotation(), entity.getScale());
    shader.loadTransformationMatrix(transformationMatrix);
}

void EntityRenderer::enableCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void EntityRenderer::disableCulling()
{
    glDisable(GL_CULL_FACE);
}