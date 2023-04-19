#pragma once
#include <string.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "runtime/include/Shader.h"
#include "Texture.h"

namespace EasyEngine {
    using namespace std;
    // extern int drawcall;
    struct Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;
        glm::vec3 biTangent;
    };

    class Mesh{
    public:
        unsigned int VAO,VBO,EBO;
        void setupMesh();
    public:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices,vector<Texture> textures);

        void draw(Shader &shader);
    };
}
