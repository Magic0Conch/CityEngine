#include "AssetManager.h"

// std::filesystem::path AssetManager::getFullPath(const std::string &relative_path) const
// {
//     return std::filesystem::absolute(relative_path);
// }
namespace EasyEngine {
    const std::string AssetManager::getFullPath(const std::string &relative_path) const{
        // return GlobalContext.get();
        
        return g_global_context.m_config_manager->getRootFolder()+("/"+relative_path); 
        // return std::filesystem::absolute(g_runtime_global_context.m_config_manager->getRootFolder() / relative_path);

    }
}