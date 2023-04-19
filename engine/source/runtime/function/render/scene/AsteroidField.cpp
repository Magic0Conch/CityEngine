#include "AsteroidField.h"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/WindowTime.h"
#include "runtime/resource/res_type/components/Model.h"
#include <cstdlib>
#include <memory>
#include <sys/types.h>

namespace EasyEngine {
    AsteroidField::AsteroidField(const std::string& shaderpath):RenderPass(shaderpath),rockModels(amount){}
    void AsteroidField::initialize(){
        srand(WindowTime::currentTimeValue);
        float radius = 50.0;
        float offset = 25.0f;
        for(unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            float angle = i*1.0/amount*360.0f;
            float displacement = (rand()%(int)(2*offset*100))/100.0f - offset;

            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. scale: scale between 0.05 and 0.25f
            float scale = (rand() % 20) / 100.0f + 0.05;
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            rockModels[i] = model;
        }  
        rockModel =make_shared<Model>(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"rock/rock.obj"));
        planetModel = make_shared<Model>(PU::getFullPath(g_global_context.m_config_manager->getDataFolder(),"planet/planet.obj"));
    }

    void AsteroidField::draw(Camera& camera){
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        // shader->setValue("model", model);
        planetModel->draw(*shader);

        unsigned int buffer;
        glBindBuffer(GL_ARRAY_BUFFER,buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &rockModels[0], GL_STATIC_DRAW);

        for (int i = 0; i<rockModel->meshes.size(); ++i) {
            unsigned int VAO = rockModel->meshes[i].VAO;
            glBindVertexArray(VAO);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(1 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,4*sizeof(glm::vec4),(void*)(3 * sizeof(glm::vec4)));
            glVertexAttribDivisor(3,1);
            glVertexAttribDivisor(4,1);
            glVertexAttribDivisor(5,1);
            glVertexAttribDivisor(6,1);
            glBindVertexArray(VAO);
            glDrawElementsInstanced(
                GL_TRIANGLES, rockModel->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
            );    
            glBindVertexArray(0);
        }
        // rockModel->draw(*shader);
    }
    AsteroidField::~AsteroidField(){

    }
}
