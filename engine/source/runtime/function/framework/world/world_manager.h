#include "runtime/function/global/global_context.h"
#include "runtime/platform/path_utility/PathUtility.h"
#include <memory>
#include <string>
#include <runtime/function/framework/Entity/entity.h>
#include <vector>
#include "../../global/global_context.h"

namespace EasyEngine {
    using PU = EasyEngine::PathUtility; 

    extern GlobalContext g_global_context;

    class WorldManager{
    public:
        vector<shared_ptr<Entity>> entity_list;
        std::vector<std::thread> threads;

        virtual ~WorldManager();
        void initialize();
        void clear();
        static WorldManager& getInstance();
        
    private:
        WorldManager(){};


        bool m_is_world_loaded = false;
        std::string m_current_world_url;

        bool loadWorld(const std::string& world_url);
        static bool loadModel(const std::string& path);
    };
}