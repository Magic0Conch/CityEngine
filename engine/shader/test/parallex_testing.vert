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
out VS_OUT{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 lightColor;
}vs_out;

uniform mat4 model;

struct Light{
    vec3 lightPos;
    vec3 lightColor;
};
uniform Light light;
uniform vec3 viewPos;

void main(){
    gl_Position = projection*view*model*vec4(aPos,1.0);
    vs_out.TexCoords = aTexCoord;
    vs_out.FragPos = vec3(model * vec4(aPos,1.0));
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    
    //re-orthogonal
    T = normalize(T - dot(T,N)*N);

    vec3 B = cross(N,T);
    mat3 TBN = mat3(T, B, N);

    vs_out.TangentLightPos = inverse(TBN)*light.lightPos;
    vs_out.TangentFragPos = inverse(TBN)*vs_out.FragPos;
    vs_out.TangentViewPos = inverse(TBN)*viewPos;
    vs_out.lightColor = light.lightColor;

}