#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 ViewPos;

const int NR_LIGHTS = 1;
const float PI = 3.1415926525;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

struct Light{
    vec3 Position;
    vec3 Color;
    float Linear;
    float Quadratic;
};
uniform Light lights[NR_LIGHTS];

vec3 getNormalFromMap(){
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}


vec3 fresnelSchlick(vec3 F_0,float cosTheta){
    //clamp(1.0 - cosTheta,0.0,1.0)
    return F_0 + (1.0 - F_0)*pow(clamp(1.0 - cosTheta,0.0,1.0),5.0);
}

float distributionGGXXTR(float roughness ,float cosTheta){ //N H
    float num = roughness*roughness;
    float denom = PI*pow((cosTheta*cosTheta*(num*num-1.0)+1.0),2);
    return num*num/denom;
}

float geomertrySchlickGGX(float cosTheta,float roughness){ //N V
    float r = roughness + 1.0;
    float k = r*r/8.0;
    float num = cosTheta;
    float denom = cosTheta*(1-k) + k;
    return num/denom;
}

float geomertrySmith(vec3 n,vec3 l,vec3 v,float roughness){
    float sub1 = geomertrySchlickGGX(max(0.0,dot(n,l)),roughness);
    float sub2 = geomertrySchlickGGX(max(0.0,dot(n,v)),roughness);
    return sub1*sub2;
}

vec3 calcSpecular(float D,vec3 F,float G,float NdotV,float NdotL){
    vec3 num = D*F*G;
    float denom = 4.0*NdotV*NdotL + 0.0001;
    return num/denom;
}

void main(){
    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic = texture(metallicMap,TexCoords).r;
    float roughness =texture(roughnessMap,TexCoords).r;
    float ao = texture(aoMap,TexCoords).r;

    vec3 normal = getNormalFromMap();
    vec3 frag2view = normalize(vec3(ViewPos-FragPos));
    vec3 F_0 = mix(vec3(0.04),albedo,metallic);
    vec3 L_o = vec3(0.0);
    for(int i = 0;i<NR_LIGHTS;++i){
        //calc specular
        vec3 frag2light = normalize(vec3(lights[i].Position - FragPos));
        vec3 h = normalize(frag2light + frag2view);
        float cosTheta = max(0.0,dot(normal,h));

        float dist = distance(FragPos,lights[i].Position);
        float attenuation = 1.0/(1.0 + lights[i].Linear * dist + lights[i].Quadratic*dist*dist);
        vec3 L_i = lights[i].Color *attenuation; 

        float HdotV=max(0.0,dot(h,frag2view));
        float NdotH = max(0.0,dot(h,normal));
        float NdotV = max(0.0,dot(normal,frag2view));
        float NdotL = max(0.0,dot(normal,frag2light));

        vec3 F = fresnelSchlick(F_0,HdotV); //TODO: why not dot(normal,frag2view)
        float NDF = distributionGGXXTR(roughness,NdotH);
        float G = geomertrySmith(normal,frag2light,frag2view,roughness);

        vec3 specualrColor = calcSpecular(NDF,F,G,NdotV,NdotL);
        // calc diffuse
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;

        kD*=1.0-metallic;
        L_o+=(kD*albedo/PI + specualrColor)*L_i*NdotL;
    }
    vec3 ambientColor = ao * albedo * vec3(0.03);
    vec3 outColor = ambientColor + L_o;
    //HDR
    outColor = outColor/(outColor + vec3(1.0));
    //gamma correct
    outColor = pow(outColor,vec3(1.0/2.2));
    
    FragColor = vec4(outColor,1.0);
}