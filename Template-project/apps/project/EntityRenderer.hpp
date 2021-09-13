#ifndef RENDERER_H
#define RENDERER_H

#include "Model.hpp"
#include "Entity.hpp"
#include "ObjectShader.hpp"
#include "./glm/glm.hpp"
#include <tuple>
#include <vector>

class EntityRenderer{
   private:
      ObjectShader shader;

      void prepareTexturedModel(TexturedModel model);
      void unbindTexturedModel();
      void prepareInstance(Entity &entity);

   public:
      EntityRenderer() = default;
      EntityRenderer(ObjectShader &shader, glm::mat4 projectionMatrix);
      void render(std::vector<std::tuple<TexturedModel, std::vector<Entity>>> &entities);
      void enableCulling();
      void disableCulling();
};

#endif