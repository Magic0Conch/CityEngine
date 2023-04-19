#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D hdrTexture;

uniform float exposure;

vec3 ReinhardToneMapping(vec3 hdrColor){
    vec3 mapped = hdrColor/(hdrColor+vec3(1.0f));
    return mapped;
}

vec3 ToneMappingWithExposure(vec3 hdrColor, float exposure){
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    return mapped;
}

vec3 GammaCorrect(vec3 color){
    const float gammaCoefficient = 2.2;
    vec3 mapped = pow(color,vec3(1.0/gammaCoefficient));
    return mapped;
}

void main(){
    vec3 hdrColor = vec3(texture(hdrTexture,TexCoords));
    vec3 ldrColor = ToneMappingWithExposure(hdrColor,exposure);

    FragColor = vec4(GammaCorrect(ldrColor),1.0);
}