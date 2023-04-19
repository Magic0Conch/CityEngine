#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture1;

const float offset = 1.0 / 300.0;  

vec3 inversion(vec3 sourceColor){
    return 1 - sourceColor;
}

vec3 grayscale(vec3 sourceColor){
    float grayval = sourceColor.r * 0.2126 + sourceColor.g*0.7152+sourceColor.b*0.0722;
    
    return vec3(grayval,grayval,grayval);
}

vec3 multiKernel(vec3 sourceColor,float kernel[9]){
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    vec3 sampleTex[9];
    for(int i = 0;i<9;++i){
        sampleTex[i]=vec3(texture(texture1,TexCoords.st+offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0;i<9;++i){
        col+=sampleTex[i]*kernel[i];
    }
    return col;
}

vec3 sharpen(vec3 sourceColor){
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    return multiKernel(sourceColor,kernel);
}

vec3 blur(vec3 sourceColor){
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    return multiKernel(sourceColor,kernel);
}

vec3 edge(vec3 sourceColor){
    float kernel[9] = float[](
        1,1,1,
        1,-8,1,
        1,1,1
    );
    return multiKernel(sourceColor,kernel);
}

void main(){
    vec3 col = texture(texture1,TexCoords).rgb;
    FragColor = vec4(col,1.0);
}