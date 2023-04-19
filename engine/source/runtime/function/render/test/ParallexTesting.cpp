#include "ParallexTesting.h"
#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/RenderPass.h"
#include "runtime/include/Shader.h"
#include "runtime/include/WindowTime.h"
// #include <GL/gl.h>
#include <map>
#include <math.h>
#include <memory>
#include <string>
namespace EasyEngine {
    ParallexTesting::ParallexTesting(const std::string& shaderPath):RenderPass(shaderPath){}
    void ParallexTesting::initialize(){

        // positions
        glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
        glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);  
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);


        glm::mat2x3 TB1 = getTB(pos1, pos2, pos3, uv1, uv2, uv3);
        glm::mat2x3 TB2 = getTB(pos1, pos3, pos4, uv1, uv3, uv4);
        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, TB1[0][0], TB1[0][1], TB1[0][2], TB1[1][0], TB1[1][1], TB1[1][2],
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, TB1[0][0], TB1[0][1], TB1[0][2], TB1[1][0], TB1[1][1], TB1[1][2],
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, TB1[0][0], TB1[0][1], TB1[0][2], TB1[1][0], TB1[1][1], TB1[1][2],

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, TB2[0][0], TB2[0][1], TB2[0][2], TB2[1][0], TB2[1][1], TB2[1][2],
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, TB2[0][0], TB2[0][1], TB2[0][2], TB2[1][0], TB2[1][1], TB2[1][2],
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, TB2[0][0], TB2[0][1], TB2[0][2], TB2[1][0], TB2[1][1], TB2[1][2]
        };
        // configure plane VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVAO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
        // configure plane VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));


        cubeTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "brick2/diffuse.bmp"));
        cubenormalTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "brick2/normal.bmp"));
        depthTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "brick2/depth.bmp"));
        colorShader = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "lighting/color.vert").c_str()
                ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "lighting/color.frag").c_str());
        uniformBlockIndex = glGetUniformBlockIndex(colorShader->ID,"Matrices");
        glUniformBlockBinding(colorShader->ID,uniformBlockIndex,0);

        colorShader->use();
        colorShader->setValue("lightColor",glm::vec3(1));
        colorShader->setValue("objectColor",glm::vec3(1));

        lightPos = glm::vec3(-1.0f, 0.0f, 1.0f);
        shader->use();
        shader->setValue("texture1",0);
        shader->setValue("normalmap",1);
        shader->setValue("depthMap",2);
        shader->setValue("height_scale",0.1f);

    }

    glm::mat2x3 ParallexTesting::getTB(glm::vec3 pos1,glm::vec3 pos2, glm::vec3 pos3 ,glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3){
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float frac = 1.0/(deltaUV1.x*deltaUV2.y - deltaUV2.x*deltaUV1.y)  ;
        glm::vec3 T,B;
        
        glm::mat2 deltaMat(deltaUV2.y,-deltaUV1.y,-deltaUV2.x,deltaUV1.x);
        glm::mat2x3 E(edge1,edge2);
        glm::mat2x3 TB = E*deltaMat*frac;
        //Tangent = TB[0] ; Bitangent = TB[1];
        return TB;
    }
    void ParallexTesting::draw(Camera& camera){
         // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture->id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubenormalTexture->id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthTexture->id);

        lightPos = glm::vec3(0*cos(WindowTime::currentTimeValue),0,0*sin(WindowTime::currentTimeValue));

        //light cube
        colorShader->use();
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(lightPos));
        model = glm::scale(model, glm::vec3(0.2));
        colorShader->setValue("model",model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader->use();
        model = glm::mat4(1);

        shader->setValue("light.lightPos",lightPos);
        shader->setValue("light.lightColor",glm::vec3(0.8f));
        shader->setValue("viewPos",camera.cameraPosition);


        //scene cube
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        // model = glm::rotate(model, (float)WindowTime::currentTimeValue, glm::vec3(0,1,0));
        shader->setValue("model",model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, (float)WindowTime::currentTimeValue*2, glm::vec3(0,1,0));
        shader->setValue("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    ParallexTesting::~ParallexTesting(){
        glDeleteVertexArrays(1,&cubeVAO);
        glDeleteVertexArrays(1,&planeVAO);
        glDeleteBuffers(1,&cubeVBO);
        glDeleteBuffers(1,&planeVBO);
    }
}