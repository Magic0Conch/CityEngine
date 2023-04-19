#include "ConfigManager.h"
#include "runtime/core/base/macro.h"
#include <algorithm>
#include <string>
namespace EasyEngine {
    void ConfigManager::initialize(const std::string &config_file_path){
        // read configs    
        std::string parentPath = BINARY_ROOT_DIR;
        std::ifstream configFile(config_file_path);
        std::string configLine;
        while (std::getline(configFile, configLine))
        {
            int seperate_pos = configLine.find_first_of('=');
            if (seperate_pos > 0 && seperate_pos < (configLine.length() - 1))
            {
                std::string name  = configLine.substr(0, seperate_pos);
                std::string value = configLine.substr(seperate_pos + 1, configLine.length() - seperate_pos - 1);
                const auto platform = getPlatformName();
                if (platform == PlatformNames::windows) {
                    std::replace(value.begin(),value.end(),'/','\\');
                }
                else if(platform == PlatformNames::linux){
                    std::replace(value.begin(),value.end(),'\\','/');
                }

                const std::string fullPath = PathUtility::getFullPath(parentPath,value);
                if (name == "BinaryRootFolder")
                {
                    m_root_folder = fullPath;
                }
                else if (name == "AssetFolder") {
                    m_asset_folder = fullPath;
                }
                else if (name == "ConfigsFolder") {
                    m_config_folder = fullPath;
                }
                else if(name == "DataFloder"){
                    m_data_folder = fullPath;
                }
                else if (name == "TextureFoloder") {
                    m_texture_folder = fullPath;
                }
                else if (name == "ShaderFolder") {
                    m_shader_folder = fullPath;                    
                }
            }
        }
    }
    void ConfigManager::setRootFolder(const std::string& path){
        m_root_folder = path;
    }

    const std::string ConfigManager::getRootFolder() const{
        return m_root_folder;
    }
    const std::string ConfigManager::getAssetFolder() const{
        return m_asset_folder;
    }

    const std::string ConfigManager::getConfigFolder() const{
        return m_config_folder;
    }
    const std::string ConfigManager::getDataFolder() const{
        return m_data_folder;
    }
    const std::string ConfigManager::getTextureFolder() const{
        return m_texture_folder;
    }
    const std::string ConfigManager::getShaderFolder() const{
        return m_shader_folder;
    }
}