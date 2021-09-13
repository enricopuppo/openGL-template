#ifndef MODEL_H
#define MODEL_H

class Model{
    private:
        int vaoID;
        int vertexCount;

    public:
        Model() = default;
        Model(int vaoID, int vertexCount);

        int getVaoID();
        int getVertexCount();
};

#endif