#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H

#include "Model.hpp"
#include "Texture.hpp"

class TexturedModel{
    private:
        Model model;
        Texture texture;

    public:
        TexturedModel() = default;
        TexturedModel(Model model, Texture texture);
        Model getModel();
        Texture getTexture();
};

#endif