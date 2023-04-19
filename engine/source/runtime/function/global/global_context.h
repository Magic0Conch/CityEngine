#pragma once
#include <memory>
#include <string>
#include "../../resource/asset_manager/AssetManager.h"
#include "../../resource/config_manager/ConfigManager.h"
namespace EasyEngine {
    class AssetManager;
    class ConfigManager;

    class GlobalContext{
    public:
        static GlobalContext get();
        void initializeContext(const std::string& config_file_path);
        void releaseContext();
    public:
        std::shared_ptr<AssetManager> m_asset_manager;
        std::shared_ptr<ConfigManager> m_config_manager;

    };
    extern GlobalContext g_global_context;
}