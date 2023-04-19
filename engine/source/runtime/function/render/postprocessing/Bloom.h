#pragma once
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <sys/types.h>
#include <vector>
namespace EasyEngine {
    class Bloom: public RenderPass{
        private:
        public:
            unsigned int hdrFBO;
            unsigned int pingpongFBOs[2];

            shared_ptr<Texture> pingpongBuffers[2];
            shared_ptr<Texture> colorBuffers[2];
            shared_ptr<Texture> textureFloor,textureBox;
            // unsigned int VAO,framebuffer,screenTexture;
            unsigned int quadVAO=0, quadVBO,cubeVAO = 0,cubeVBO;
            std::vector<glm::vec3> lightPositions;
            std::vector<glm::vec3> lightColors;

            shared_ptr<Shader> shaderBlur,shaderLight,shaderBloomFinal;
        public:
            Bloom();
            virtual void initialize();
            virtual void draw(Camera& camera);
            void renderCube();
            void renderQuad();

            virtual ~Bloom();

    };
}
