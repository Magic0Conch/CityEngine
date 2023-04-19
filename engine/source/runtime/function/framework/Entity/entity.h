#pragma once
#include "runtime/resource/res_type/components/Model.h"
#include <memory>
#include <string>
namespace EasyEngine {
    class Entity{
        public:
            std::shared_ptr<Model> model;
            Entity() = delete;
            Entity(const string& model_path);
    };

}