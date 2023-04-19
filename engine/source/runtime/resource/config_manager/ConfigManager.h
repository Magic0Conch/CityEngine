#pragma once
#include <string>
#include <fstream>
#include "runtime/platform/path_utility/PathUtility.h"
namespace EasyEngine {
    class PathUtility;
    class ConfigManager{
    public:
        void initialize(const std::string& config_file_path);
        const std::string getRootFolder() const;
        const std::string getAssetFolder() const;
        const std::string getConfigFolder() const;
        const std::string getDataFolder() const;
        const std::string getTextureFolder() const;
        const std::string getShaderFolder() const;
        void setRootFolder(const std::string&);
    private:
        std::string m_root_folder;
        std::string m_asset_folder;
        std::string m_config_folder;
        std::string m_data_folder;
        std::string m_texture_folder;
        std::string m_shader_folder;

    };
}