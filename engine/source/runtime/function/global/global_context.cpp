#include "global_context.h"
namespace EasyEngine {
    GlobalContext g_global_context;

    GlobalContext GlobalContext::get(){
        static GlobalContext instance;
        return instance;
    }
    void GlobalContext::initializeContext(const std::string& config_file_path){
        m_config_manager = std::make_shared<ConfigManager>();
        m_config_manager->initialize(config_file_path);
        m_asset_manager = std::make_shared<AssetManager>();
    }
    void GlobalContext::releaseContext(){
        //do nothing
    }

}

