#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

const int kernelSize = 64;
const float radius = 0.5;
const float bias = 0.025;
const float power = 1.0;

uniform vec3 samples[kernelSize];
layout(std140) uniform Matrices{
    mat4 projection;
};

const vec2 noiseScale = vec2(800.0/4,600.0/4);
void main(){
    vec3 fragPos = texture(gPosition,TexCoords).xyz;
    vec3 normal = texture(gNormal,TexCoords).xyz;
    vec3 randomVec = texture(texNoise,TexCoords*noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal*dot(randomVec,normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent,bitangent,normal);

    //rotate by TBN matrix
    float occlusion = 0.0;
    for (int i = 0; i<kernelSize;++i) {
        vec3 samplePos = TBN * samples[i]; //Tto viewPos? TBN
        samplePos = fragPos + samplePos*radius;
        vec4 offset = projection * vec4(samplePos,1.0);
        offset.xyz/=offset.w;
        offset.xyz = offset.xyz*0.5+0.5;
        float sampleDepth = texture(gPosition,offset.xy).z;
        //Get rid off edge artifacts.
        float rangeCheck = smoothstep(0.0,1.0,radius/abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth>=samplePos.z + bias?1.0:0.0)*rangeCheck;
    }
    occlusion = 1.0 - (occlusion/kernelSize);
    FragColor = pow(occlusion,power);


}