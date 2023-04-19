#include "InstancingTesting.h"
#include "glm/fwd.hpp"
#include "runtime/function/render/RenderPass.h"

namespace EasyEngine {
    InstancingTesting::InstancingTesting(const std::string& shaderpath):RenderPass(shaderpath),translations(100){}
    void InstancingTesting::initialize(){
        
        float points[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f		
        };   
        int index = 0;
        float offset = 0.1f;
        for(int y = -10; y < 10; y += 2)
        {
            for(int x = -10; x < 10; x += 2)
            {
                glm::vec2 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translations[index] = translation;
                // shader->setValue("offsets["+std::to_string(index)+"]",(translations[index]));
                ++index;
            }
        }  
        unsigned int VBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        int pointSize = sizeof(points);
        glBufferData(GL_ARRAY_BUFFER,pointSize,&points,GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)8);
        glEnable(GL_PROGRAM_POINT_SIZE);
        unsigned int instanceVBO;
        glGenBuffers(1,&instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)*100,&translations[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
        glVertexAttribDivisor(2,1);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }
    void InstancingTesting::draw(Camera& camera){
        shader->use();
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6,100);
        glBindVertexArray(0);
    }
    InstancingTesting::~InstancingTesting(){}
}
