#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;
layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    vec3 viewPos;
};
out VS_OUT {
    vec2 texCoords;
    vec3 FragPos;
    vec3 Normal;
    vec3 viewPos;
} vs_out;

void main(){
    vs_out.FragPos = vec3(model * vec4(aPos,1.0));
    vs_out.texCoords=aTexCoords;
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.viewPos = viewPos;
    //normal matrix
    gl_Position=projection*view*model*vec4(aPos,1.0f);
}