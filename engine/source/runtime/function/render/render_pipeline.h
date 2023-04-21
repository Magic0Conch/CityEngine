#include "runtime/function/render/lighting/DiffuseLighting.h"
#include <memory>
namespace EasyEngine {
    class RenderPipeline{
    public:
        std::shared_ptr<DiffuseLighting> m_diffuse_light_pass;

    public:
        void uploadGlobalRenderResource();
        void initialize();
        void forwardRender(Camera& camera);
    };
}