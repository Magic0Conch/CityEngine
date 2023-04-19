#version 330 core

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 TexCoords;

struct Light{
    vec3 Position;
    vec3 Color;
    float Linear;
    float Quadratic;
    float Radius;    
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main(){
    vec3 FragPos = texture(gPosition,TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal,TexCoords).rgb);
    vec3 Albedo = texture(gAlbedoSpec,TexCoords).rgb;
    float specularCoeff = texture(gAlbedoSpec,TexCoords).a;

    vec3 ambient = Albedo*0.1;
    vec3 viewDir = normalize(viewPos-FragPos);

    vec3 lighting = ambient;

    for(int i = 0;i<NR_LIGHTS;i++){
        float dist = distance(FragPos,lights[i].Position);
        if(dist<lights[i].Radius){
            vec3 lightDir = normalize(lights[i].Position-FragPos);
            vec3 diffuse = max(0.0f,dot(Normal,lightDir))*Albedo*lights[i].Color;
            vec3 specualr = pow(max(0.0f,dot(normalize(viewDir+lightDir),Normal)),16)*lights[i].Color*specularCoeff;

            float attenuation = 1.0/(1.0+ lights[i].Linear*dist + lights[i].Quadratic*dist*dist);
            diffuse*=attenuation;
            specualr*=attenuation;
            lighting+=diffuse+specualr;
        }
    }
    FragColor = vec4(lighting,1.0f);

}

