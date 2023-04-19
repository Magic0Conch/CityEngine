#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blurLightTexture;
uniform float exposure;

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
    vec3 hdrColor = texture(scene,TexCoords).rgb;
    vec3 blurColor = texture(blurLightTexture,TexCoords).rgb;
    hdrColor+=blurColor;
    
    vec3 ldrColor = ToneMappingWithExposure(hdrColor,exposure);
    vec3 result = GammaCorrect(ldrColor);
    FragColor = vec4(result,1.0);

}