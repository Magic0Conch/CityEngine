#version 330 core

out float FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;

void main(){
    vec2 texelSize = 1.0/vec2(textureSize(screenTexture,0));
    float result = 0.0f;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(x*1.0, y*1.0) * texelSize;
            result += texture(screenTexture, TexCoords + offset).r;
        }
    }
    FragColor = result/(4.0*4.0);
}