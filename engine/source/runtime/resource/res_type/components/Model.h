#pragma once
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <memory>
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "runtime/include/Shader.h"
#include "runtime/resource/res_type/components/Texture.h"

namespace EasyEngine {
static int loadedCount = 0;

class Model{
    public:
        vector<Texture> textures_loaded; 
        vector<Mesh> meshes;
        string directory;
        vector<Texture_Info> texture_infos;
        

    public:
        void loadModel(const string& path);
        void setupModel();

    public:
        Model(const string& path);
        void draw(Shader &shader);
    private:
        void processNode(aiNode *node,const aiScene *scene);
        Mesh processMesh(aiMesh *mesh,const aiScene *scene);
        vector<string> loadMaterialTexturesPaths(aiMaterial *mat,aiTextureType type);
};

}