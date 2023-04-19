#pragma once
#include <string>
#include "runtime/include/ImageProcessing.h"
#include <string>
#include <glad/glad.h>
#include <vector>

namespace EasyEngine{

class Texture_Info{
public:
    string type_name;
    vector<string> texture_paths;
public:
    Texture_Info(string,vector<string>);
};

enum TextureInternalFormat{
    DEFAULT, GAMMACORRECTION, HDR
};
class Texture{
private:
public:
    unsigned int id;
    std::string type;
    std::string path;
    Texture(const std::string& path,const GLenum& textureType = GL_TEXTURE_2D,TextureInternalFormat internalFormat = DEFAULT,bool inverseY = false);
    Texture(const std::vector<std::string>& paths,const GLenum& textureType = GL_TEXTURE_CUBE_MAP);
    Texture(const int width,const int height,const int component,TextureInternalFormat internalFormat = DEFAULT,bool inverseY = false,bool bgr = false);
    void getTextureFormat(const int components,GLenum &outFormatInternal,GLenum &outFormatSource,const TextureInternalFormat targetFormat,const bool bgr);
};

}