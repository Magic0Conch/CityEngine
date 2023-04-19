#include "entity.h"
#include <memory>
#include <string>

using namespace EasyEngine;

Entity::Entity(const std::string& model_path){
    model = make_shared<Model>(model_path);
}
