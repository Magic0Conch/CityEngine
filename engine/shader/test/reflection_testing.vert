#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

out vec3 Positon;
out vec3 Normal;

uniform mat4 model;
layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

void main(){
    Normal = mat3(transpose(inverse(model))) * aNormal; 
    Positon = vec3(model*vec4(aPos,1.0));
    gl_Position = projection*view*model*vec4(aPos,1.0);    
}