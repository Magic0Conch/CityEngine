#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

float near=0.1;
float far=100.0;

void main(){
    float depth = gl_FragCoord.z;
    float ndc = depth*2.0-1.0;
    float linearDepth = (2.0*near*far)/(far+near-ndc*(far-near));
    FragColor = vec4(vec3(linearDepth/far),1.0f);
}