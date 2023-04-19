#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

out VS_OUT {
    vec3 normal;
    mat4 projection;
} vs_out;

void main(){
    //normal matrix
    gl_Position=view*model* vec4(aPos,1.0f);
    vs_out.projection = projection;
    mat3 normalMatrix = mat3(transpose(inverse(view*model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}