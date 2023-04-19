#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoords;
layout(location = 3)in mat4 instanceMatrix;

layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};
out vec2 TexCoords;
// uniform vec2 offsets[100];
void main(){
    // vec2 offset = offsets[gl_InstanceID];
    gl_Position=projection*view*instanceMatrix*vec4(aPos,1.0f);
    TexCoords = aTexCoords;
}