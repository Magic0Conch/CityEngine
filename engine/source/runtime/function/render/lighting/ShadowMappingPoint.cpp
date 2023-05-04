#include "ShadowMappingPoint.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "runtime/include/EngineWindow.h"
#include "runtime/include/Shader.h"
#include "runtime/resource/res_type/common/PostProcessingParameter.h"
#include "runtime/resource/res_type/components/Camera.h"
#include "runtime/resource/res_type/components/Texture.h"
#include <memory>
#include <string>



using namespace EasyEngine;
//To get visuals similar to Phong shading the specular shininess exponent has to be set a bit higher. 
//A general rule of thumb is to set it between 2 and 4 times the Phong shininess exponent. 
ShadowMappingPoint::ShadowMappingPoint(const string& shaderPath)
: RenderPass(shaderPath){
    shaderDepth = std::make_shared<Shader>(PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/depth_cube.vert").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/depth_cube.geom").c_str()
            ,PU::getFullPath(g_global_context.m_config_manager->getShaderFolder(), "common/depth_cube.frag").c_str());
    uniformBlockIndex = glGetUniformBlockIndex(shaderDepth->ID,"Matrices");
    glUniformBlockBinding(shaderDepth->ID,uniformBlockIndex,0);
}

void ShadowMappingPoint::initialize(){

    // load textures
    // -------------
    floorTexture = make_shared<Texture>(PU::getFullPath(g_global_context.m_config_manager->getTextureFolder(), "default/woodfloor.jpg"),GL_TEXTURE_2D,DEFAULT);

    //----
    glGenFramebuffers(1, &depthMapFBO);  
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for(int i = 0;i<6;++i){
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);}

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    GLuint status = glCheckFramebufferStatus(depthMapFBO);
    if(status != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //light pos fixed, no need to be updated every frame.
    lightCamera = make_shared<Camera>(CameraType::PERSPECTIVE);
    lightCamera->nearPlane = 1.0f;
    lightCamera->farPlane = 25.0f;
    lightCamera->fov = 90.0;
    lightCamera->aspect = SHADOW_WIDTH*1.0/SHADOW_HEIGHT;
    //pass the parameter to postprocessing shader
    // parameters.emplace_back(make_shared<PostProcessingParameterFloat>("near_plane",lightCamera->nearPlane));
    // parameters.emplace_back(make_shared<PostProcessingParameterFloat>("far_plane",lightCamera->farPlane));
    // parameters.emplace_back(make_shared<PostProcessingParameterInt>("texture1",0));
    // parameters.emplace_back(make_shared<PostProcessingParameterInt>("framebuffer",depthMapFBO));
}
void ShadowMappingPoint::drawCube()
{
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void ShadowMappingPoint::drawScene(Shader& curShader){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(5.0f));
    shader->setValue("model", model);
    glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    shader->setValue("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
    drawCube();
    shader->setValue("reverse_normals", 0); // and of course disable it
    glEnable(GL_CULL_FACE);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader->setValue("model", model);
    drawCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader->setValue("model", model);
    drawCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader->setValue("model", model);
    drawCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader->setValue("model", model);
    drawCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader->setValue("model", model);
    drawCube();
}

void ShadowMappingPoint::draw(Camera& camera){
    lightCamera->setCameraPosition(glm::vec3(0.0f, 0.0f, static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0)));
    // lightCamera->setCameraRotation(glm::vec3(2.0f, -4.0f, 1.0f),glm::vec3(0,1,0));

    std::vector<glm::mat4> shadowTransforms;

    shadowTransforms.push_back(lightCamera->getProjectionMatrix() * 
                    glm::lookAt(lightCamera->cameraPosition, lightCamera->cameraPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));

    shadowTransforms.push_back(lightCamera->getProjectionMatrix() * 
                    glm::lookAt(lightCamera->cameraPosition, lightCamera->cameraPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));

    shadowTransforms.push_back(lightCamera->getProjectionMatrix() * 
                    glm::lookAt(lightCamera->cameraPosition, lightCamera->cameraPosition + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(lightCamera->getProjectionMatrix() * 
                    glm::lookAt(lightCamera->cameraPosition, lightCamera->cameraPosition + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(lightCamera->getProjectionMatrix() * 
                    glm::lookAt(lightCamera->cameraPosition, lightCamera->cameraPosition + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(lightCamera->getProjectionMatrix() * 
                    glm::lookAt(lightCamera->cameraPosition, lightCamera->cameraPosition + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // 1. render to depth map
    shaderDepth->use();
    for(int i = 0;i<6;++i)
        shaderDepth->setValue("shadowTransforms["+to_string(i)+"]",shadowTransforms[i]);
    shaderDepth->setValue("far_plane",lightCamera->farPlane);
    shaderDepth->setValue("lightPos",lightCamera->cameraPosition);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    // glCullFace(GL_FRONT);
        glClear(GL_DEPTH_BUFFER_BIT);
        drawScene(*shaderDepth);
    // glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  


    // 2. then render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, EngineWindow::getInstance().viewportWidth, EngineWindow::getInstance().viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ConfigureShaderAndMatrices();
    shader->use();
    shader->setValue("far_plane",lightCamera->farPlane);
    shader->setValue("diffuseTexture",0);
    shader->setValue("depthMap",1);
    shader->setValue("lightPos",lightCamera->cameraPosition);
    shader->setValue("shadows",0);
    shader->setValue("viewPos",camera.cameraPosition);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,floorTexture->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubeMap);
    
    drawScene(*shader);
    glActiveTexture(GL_TEXTURE0);
    // RenderScene();
    
}
