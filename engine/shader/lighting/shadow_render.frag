#version 330 core
#define AMBIENT_RATE 0.3

out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 blingPhong(float shadow){
    vec3 col = vec3(texture(diffuseTexture,fs_in.TexCoords));
    vec3 point2EyeDir = normalize(viewPos - fs_in.FragPos);
    vec3 point2LightDir = normalize(lightPos - fs_in.FragPos);
    vec3 h = normalize(point2EyeDir + point2LightDir);

    vec3 lightColor = vec3(0.8);
    vec3 ambiendColor = lightColor * AMBIENT_RATE;
    
    float diffuse = max (dot(point2LightDir,fs_in.Normal),0.0);
    vec3 diffuseColor = lightColor * diffuse;

    float specular = pow(max(dot(h,fs_in.Normal),0.0),64.0);
    vec3 specularColor = specular*lightColor;


    vec3 result = (ambiendColor + (1-shadow)*(diffuseColor + specularColor))*col;
    return result;
}

float shadowCalculation(){
    vec3 projectionCoords = fs_in.FragPosLightSpace.xyz/fs_in.FragPosLightSpace.w;
    projectionCoords = projectionCoords*0.5+0.5;
    float depthFromLight = texture(shadowMap,projectionCoords.xy).r;
    float depthCurrent = projectionCoords.z;
    if(depthCurrent>1.0)
        return 0.0;
    float bias = max(0.04*(1.0-dot(fs_in.Normal,normalize(lightPos - fs_in.FragPos))),0.004);

    float result = 0.0;
    vec2 texelSize = 1.0/textureSize(shadowMap,0);
    //pcf
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projectionCoords.xy + vec2(x, y) * texelSize).r; 
            result += depthCurrent - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    result/=9.0;
    return result;
}

void main(){
    float shadow = shadowCalculation();
    FragColor = vec4(blingPhong(shadow),1.0);

}
