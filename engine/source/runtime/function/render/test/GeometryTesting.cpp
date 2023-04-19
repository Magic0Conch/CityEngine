#include "GeometryTesting.h"
#include "runtime/function/render/RenderPass.h"

namespace EasyEngine {
    GeometryTesting::GeometryTesting(const std::string& vertShaderPath,const std::string& geomShaderPath,const std::string& fragShaderPath):
    RenderPass(vertShaderPath,geomShaderPath,fragShaderPath){}
    void GeometryTesting::initialize(){
        float points[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
        };   
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
        glPointSize(5);
        glBindVertexArray(0);
    }
    void GeometryTesting::draw(Camera& camera){
        shader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 4);
        glBindVertexArray(0);
    }
    GeometryTesting::~GeometryTesting(){

    }
}
