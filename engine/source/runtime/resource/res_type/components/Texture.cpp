#include "Texture.h"
#include <opencv2/imgcodecs.hpp>
#include <string>

namespace EasyEngine {
    Texture_Info::Texture_Info(string type_name,vector<string> texture_paths):type_name(type_name),texture_paths(texture_paths){}

    void Texture::getTextureFormat(const int components,GLenum &outFormatInternal,GLenum &outFormatSource,const TextureInternalFormat targetFormat,const bool bgr){
        switch (components) {
            case 1:
                outFormatInternal = GL_RED;//internalFormat 
                outFormatSource = GL_RED;
                break;
            case 3:
                switch (targetFormat) {
                    case DEFAULT:
                        outFormatInternal = GL_RGB;
                        break;
                    case GAMMACORRECTION:
                        outFormatInternal = GL_SRGB;
                        break;
                    case HDR:
                        outFormatInternal = GL_RGB16F;
                        break;
                }                
                outFormatSource = bgr? GL_BGR:GL_RGB;
                break;
            case 4:
                switch (targetFormat) {
                    case DEFAULT:
                        outFormatInternal = GL_RGBA;
                        break;
                    case GAMMACORRECTION:
                        outFormatInternal = GL_SRGB_ALPHA;
                        break;
                    case HDR:
                        outFormatInternal = GL_RGBA16F;
                        break;
                }
                outFormatSource = bgr? GL_BGRA:GL_RGBA;
                break;
        }
    }

    Texture::Texture(const std::string& path,const GLenum& textureType,TextureInternalFormat internalFormat,bool inverseY){
        cv::Mat data = ImageProcessing::readImageByPath(path,IMREAD_UNCHANGED,inverseY);
        if (data.empty()) {
            cout << "Texture::Texture: load texture from "+path + " failed";
            return;
        }
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        //set the texture wrapping/filtering options (on the currently bound texture object)

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format_target,format_source;
        getTextureFormat(data.channels(), format_target, format_source, internalFormat,true);
        glTexImage2D(GL_TEXTURE_2D,0,format_target,data.cols,data.rows,0,format_source,GL_UNSIGNED_BYTE,data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        data.release();
    }

    Texture::Texture(const std::vector<std::string>& paths,const GLenum& textureType){
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        int t = 0;
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
        for (const std::string& path:paths) {
            cv::Mat data = ImageProcessing::readImageByPath(path);
            if (data.empty()) {
                cout << "Texture::Texture: load texture from "+path + " failed";
                return;
            }
            GLenum format_target,format_source;
            getTextureFormat(data.channels(), format_target, format_source, DEFAULT,true);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + t++,0,format_target,data.cols,data.rows,0,format_source,GL_UNSIGNED_BYTE,data.data);
            data.release();
        }
    }



    Texture::Texture(const int width,const int height,const int components,TextureInternalFormat internalFormat,bool inverseY,bool bgr){
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D,id);
        GLenum format_internal,format_source;
        getTextureFormat(components, format_internal, format_source, internalFormat,false);
        glTexImage2D(GL_TEXTURE_2D,0,format_internal,width,height,0,format_source,GL_FLOAT,NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);        
    }

}