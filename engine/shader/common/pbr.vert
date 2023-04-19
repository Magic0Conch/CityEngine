#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 ViewPos;

layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    vec3 viewPos;
};

void main(){
    gl_Position = projection*view*model*vec4(aPos,1.0);
    TexCoords = aTexCoords;
    Normal = aNormal;
    FragPos = (model * vec4(aPos,1.0)).rgb;
    ViewPos = viewPos;
}