#pragma once
#include <sys/types.h>
namespace EasyEngine {

    class RenderShape{
        private:
            RenderShape();
            unsigned int cubeVAO = 0,quadVAO = 0,sphereVAO = 0,indexCount;
        public:
            RenderShape(const RenderShape&) = delete;
            RenderShape& operator=(const RenderShape&) = delete;
            RenderShape(RenderShape&&) = delete;
            RenderShape& operator=(RenderShape&&) = delete;

            static RenderShape& instance();

            void renderCube();
            void renderQuad();
            void renderSphere();

    };

}