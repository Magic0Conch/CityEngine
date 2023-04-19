#pragma once
#include "runtime/function/render/RenderPass.h"
#include <memory>
#include <string.h>
#include <glad/glad.h>
#include <sys/types.h>
#include "runtime/resource/res_type/components/Texture.h"
namespace EasyEngine {
    class ReflectionTesting:public RenderPass{

        public:
            unsigned int cubeVAO, cubeVBO,textureSkybox;
            shared_ptr<Texture> skyboxTexture;

            ReflectionTesting(const std::string& shaderPath);
            virtual void draw(Camera& camera);
            virtual void initialize();
            virtual ~ReflectionTesting();

    };
}
