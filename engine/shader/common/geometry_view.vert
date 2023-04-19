#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;


uniform mat4 model;
layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    vec3 viewPos;
};

void main(){
    FragPos = vec3(view*model * vec4(aPos,1.0f));
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos,1.0f);
    Normal = normalize(mat3(transpose(inverse(view*model))) * aNormal);
}