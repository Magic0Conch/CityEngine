#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;
layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};
out VS_OUT {
    vec2 texCoords;
} vs_out;

void main(){
    //normal matrix
    gl_Position=projection*view*model*vec4(aPos,1.0f);
    vs_out.texCoords=aTexCoords;
}