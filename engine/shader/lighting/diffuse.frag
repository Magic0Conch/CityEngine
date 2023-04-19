#version 330 core
#define NR_POINT_LIGHT 4
#define NR_SPOT_LIGHT 1

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct DirectionalLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directionalLight;

vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir){

    vec3 texture_diffuse1Col = vec3(texture(material.texture_diffuse1,TexCoords));
    vec3 ambientColor = light.ambient*texture_diffuse1Col*material.ambient;
    
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diff*light.diffuse*texture_diffuse1Col*material.diffuse;
    
    // vec3 viewDir = normalize(viewPos-FragPos);
    vec3 h = normalize(lightDir + viewDir);
    float spec = pow(max(dot(h,normal),0.0) ,material.shininess);
    vec3 specularColor = spec*material.specular*light.specular*vec3(texture(material.texture_specular1,TexCoords));
    // specularColor = vec3(0.5f)*vec3(texture(material.texture_specular1,TexCoords))*material.specular*light.specular*spec;

    return ambientColor+diffuseColor+specularColor;
}



void main(){
    vec3 resultColor = vec3(0.0f); //vec3 resultColor = vec3(0.0);
    vec3 viewDir = FragPos - viewPos;
    vec3 normal = normalize(Normal);
    resultColor+=CalcDirLight(directionalLight,normal,viewDir);
    FragColor = vec4(resultColor,1.0f);
}