#include "world_manager.h"
#include "runtime/platform/path_utility/PathUtility.h"
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <thread>
#include <vector>

using namespace EasyEngine;

WorldManager::~WorldManager(){

}

WorldManager& WorldManager::getInstance(){
    static WorldManager instance;
    return instance;
}

void WorldManager::initialize(){
    loadWorld("C:\\Data\\snow3\\Productions\\Production_2\\Data");

}
void WorldManager::clear(){

}
void WorldManager::tick(){

}

bool WorldManager::loadModel(const std::string& path){
    shared_ptr<Entity> entity_ptr = make_shared<Entity>(path);
    WorldManager::getInstance().entity_list.emplace_back(entity_ptr);
    return false;
}

bool WorldManager::loadWorld(const std::string& world_url){
    auto paths = PathUtility::getAllPathWithExt(world_url, ".obj");
    
    for (auto path:paths) {
        loadModel(path);
        // threads.emplace_back(std::thread(WorldManager::getInstance().loadModel,path));
    }
    for (auto& thread : threads) {
        // thread.join();
    }
    // shared_ptr<Entity> entity_ptr = make_shared<Entity>("E:\\Files\\Work\\Project\\CityEngine\\build\\bin\\asset\\data\\backpack\\backpack.obj");
    return true;
}


