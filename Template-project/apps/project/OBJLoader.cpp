#include "OBJLoader.hpp"
#include "ModelLoader.hpp"
#include "Model.hpp"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "./glm/glm.hpp"

std::vector<std::string> OBJLoader::lineToTokens(std::string line, std::string separator){
    std::vector<std::string> tokens;    // generate an empty array of string pieces
    size_t loc = 0;
    while((loc = line.find(separator)) != std::string::npos){
        tokens.push_back(line.substr(0, loc));     // take a piece of string from the beginning to the first space
        line.erase(0, loc + separator.length());;     // consume the taken part from the line
    }
    tokens.push_back(line);   // add the last part from the last space and the end of the string
    return tokens;
}

void OBJLoader::processVertex(std::vector<std::string> &vertexData, std::vector<GLuint> &indices, std::vector<glm::vec2> &fileUVs, std::vector<glm::vec3> &fileNormals, std::vector<GLfloat> &uv, std::vector<GLfloat> &normals){
        int vertexPointer = std::atoi(vertexData[0].c_str()) - 1;   // extract the vertex index
        indices.push_back(vertexPointer);   // push it into indices array

        glm::vec2 uvCoord = fileUVs[std::atoi(vertexData[1].c_str()) - 1];   // extract the uv coordinates at the found index
        uv[vertexPointer * 2] = uvCoord[0];        // copy in the vertexPointer position the two values
        uv[vertexPointer * 2 + 1] = uvCoord[1];
        
        glm::vec3 normalsCoord = fileNormals[std::atoi(vertexData[2].c_str()) - 1];     // do the same with the normals
        normals[vertexPointer * 3] = normalsCoord[0];
        normals[vertexPointer * 3 + 1] = normalsCoord[1];
        normals[vertexPointer * 3 + 2] = normalsCoord[2];
}

Model OBJLoader::loadObjModel(std::string path, ModelLoader loader){
    // temporary structures for extracted data from file
    std::vector<glm::vec3> fileVertices;
    std::vector<glm::vec3> fileNormals;
    std::vector<glm::vec2> fileUVs;
    std::vector<GLuint> indices;
    // final sorted structures
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uv;

    FILE* file;
    char* fileLine = NULL;
    size_t len = 0;
    bool allocated = false;
    
    file = fopen(path.c_str(), "r");    // open the file
    if(!file)
        printf("Cannot open OBJ file!\n");
    else{
        while(getline(&fileLine, &len, file) > 0){      // while the function reads a positive number of characters from the current line
            std::string stringFileLine = std::string(fileLine);     // convert the cstring into string
            std::vector<std::string> tokens = lineToTokens(stringFileLine, " ");

            // parse the tokens depending on the initial part of the line
            if(tokens[0] == "v"){       // if vertices generate a vector of 3
                fileVertices.push_back(glm::vec3(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str())));
            }
            else if(tokens[0] == "vt"){     // if texture coordinates generate a vector of 2
                fileUVs.push_back(glm::vec2(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())));
            }
            else if(tokens[0] == "vn"){     // if normals generate a vector of 3
                fileNormals.push_back(glm::vec3(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str())));
            }
            else if(tokens[0] == "f"){      // we reached faces, three sets of three indices each, slash-separated
                if(!allocated){     // preallocate the uv and normal vectors to access all the locations
                    uv.resize(fileVertices.size() * 2, 0.0);
                    normals.resize(fileVertices.size() * 3, 0.0);
                    allocated = true;
                }
                for(int i=1; i<4; ++i){     // for each group of indices 
                    std::vector<std::string> tokensIndices = lineToTokens(tokens[i], "/");   // split the numbers looking for the slash
                    processVertex(tokensIndices, indices, fileUVs, fileNormals, uv, normals);   // arrange the sorted vectors using the extracted indices
                }
            }
        }
    }
    fclose(file);

    vertices.resize(fileVertices.size() * 3, 0.0);      // preallocate the vertices vector
    int vertexPointer = 0;
    
    for(size_t i=0; i<fileVertices.size(); ++i){       // fill with the vertices coordinates from the file
        vertices[vertexPointer++] = fileVertices[i][0];
        vertices[vertexPointer++] = fileVertices[i][1];
        vertices[vertexPointer++] = fileVertices[i][2];
    }
    return loader.loadToVAO(vertices, indices, uv, normals);
}