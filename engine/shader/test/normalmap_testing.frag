#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D texture1;
uniform sampler2D normalmap;

struct Light{
    vec3 lightPos;
    vec3 lightColor;
};
uniform Light light;
uniform vec3 viewPos;

vec3 blingPhone(){
    vec3 ansColor;
    vec3 ambientColor = light.lightColor*0.03;

    vec3 normal = normalize(TBN * vec3(texture(normalmap,TexCoords)));
    normal = normalize((normal*2.0)-1.0);

    vec3 frag2lightDir = normalize(light.lightPos - FragPos);
    float diffuseCoefficient = max(dot(normal,frag2lightDir),0.0);
    vec3 diffuseColor = diffuseCoefficient * light.lightColor;
    

    vec3 frag2viewDir = normalize(viewPos - FragPos);
    vec3 h = normalize(frag2viewDir + frag2lightDir);
    float specularCoefficient = max(dot(normal,h),0.0);
    vec3 specularColor = pow(specularCoefficient,128) * light.lightColor;

    float dist = distance(FragPos,light.lightPos);
    float attenunation = 1.0/(1 + dist*0.09 + dist*dist*0.032);

    ansColor = ambientColor + (diffuseColor + specularColor)*attenunation;

    return ansColor;
}

void main()
{             
    vec3 texColor = vec3(texture(texture1, TexCoords));
    FragColor = vec4(texColor * blingPhone(),1.0);
}