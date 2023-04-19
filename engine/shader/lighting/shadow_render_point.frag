#version 330 core
#define AMBIENT_RATE 0.3

out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far_plane;
uniform bool shadows;
vec3 blingPhong(float shadow){
    vec3 col = vec3(texture(diffuseTexture,fs_in.TexCoords));
    vec3 point2EyeDir = normalize(viewPos - fs_in.FragPos);
    vec3 point2LightDir = normalize(lightPos - fs_in.FragPos);
    vec3 h = normalize(point2EyeDir + point2LightDir);

    vec3 normal = normalize(fs_in.Normal);

    vec3 lightColor = vec3(0.3);
    vec3 ambiendColor = lightColor * AMBIENT_RATE;
    
    float diffuse = max(dot(point2LightDir,normal),0.0);
    vec3 diffuseColor = lightColor * diffuse;

    float specular = pow(max(dot(h,normal),0.0),64.0);
    vec3 specularColor = specular*lightColor;


    vec3 result = (ambiendColor + (1- (shadows? shadow:0.0))*(diffuseColor + specularColor))*col;
    return result;
}

float shadowCalculation(vec3 fragPos){
    vec3 light2FragDir = normalize(fragPos - lightPos);
    float depthFromLight = texture(depthMap,light2FragDir).r * far_plane;
    float depthCurrent = length(light2FragDir);

    
    float bias = 0.05;

    float result = 0.0;
    result = depthCurrent - bias > depthFromLight ? 1.0 : 0.0;        
    vec2 texelSize = 1.0/textureSize(depthMap,0);

    FragColor = vec4(vec3(depthFromLight / far_plane), 1.0);  
    //pcf
    // for(int x = -1; x <= 1; ++x)
    // {
    //     for(int y = -1; y <= 1; ++y)
    //     {
    //         float pcfDepth = texture(shadowMap, projectionCoords.xy + vec2(x, y) * texelSize).r; 
    //         result += depthCurrent - bias > pcfDepth ? 1.0 : 0.0;        
    //     }    
    // }
    // result/=9.0;
    return result;
}

void main(){
    float shadow = shadowCalculation(fs_in.FragPos);
    // FragColor = vec4(blingPhong(shadow),1.0);
}
