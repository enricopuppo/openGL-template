#include "TexturedModel.hpp"
#include "Model.hpp"
#include "Texture.hpp"

TexturedModel::TexturedModel(Model model, Texture texture){
    this->model = model;
    this->texture = texture;
}

Model TexturedModel::getModel(){
    return model;
}

Texture TexturedModel::getTexture(){
    return texture;
}