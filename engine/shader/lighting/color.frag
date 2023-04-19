#version 330 core
layout (location = 0)out vec4 FragColor;
// in vec2 TexCoords;
// uniform sampler2D texture0;

uniform vec3 objectColor;
uniform vec3 lightColor;


void main(){
    FragColor = vec4(objectColor*lightColor,1.0);
}