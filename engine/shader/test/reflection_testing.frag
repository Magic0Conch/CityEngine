#version 330 core
out vec4 FragColor;

in vec3 Positon;
in vec3 Normal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

vec3 getReflect(){
    vec3 I = normalize(Positon - cameraPosition);
    vec3 R = reflect(I,normalize(Normal));
    vec3 texColor = texture(skybox, R).xyz;
    return texColor;
}

vec3 getRefraction(){
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Positon - cameraPosition);
    vec3 R = refract(I,normalize(Normal),ratio);
    vec3 texColor = texture(skybox, R).xyz;
    return texColor;
}

void main()
{   
    FragColor = vec4(getRefraction(),1);
}