#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in vec3 aTangent;
layout(location=4) in vec3 aBitangent;
layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};
out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 model;

void main(){
    gl_Position = projection*view*model*vec4(aPos,1.0);
    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos,1.0));
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));

    T = normalize(T - dot(T,N)*N);
    vec3 B = cross(N,T);
    
    TBN = mat3(T, B, N);
}