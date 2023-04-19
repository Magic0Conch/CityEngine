#version 330 core

out vec4 FragColor;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;



void main(){
    vec3 col = vec3(texture(material.texture_diffuse1,vec2(0.5,0.5)));
    if(col.r<0.5)
        col = vec3(0.0275, 0.8431, 0.1647);
    FragColor = vec4(col,1.0f);
}