#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;
void main(){
    vec3 Color = vec3(texture(material.texture_diffuse1,TexCoords));
    FragColor = vec4(Color,1.0);
}