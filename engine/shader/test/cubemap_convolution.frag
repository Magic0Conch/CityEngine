#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main(){
    vec3 normal = normalize(localPos);
    vec3 irradiance = vec3(0.0);

    vec3 t = vec3(0.0,1.0,0.0);
    vec3 b = normalize(cross(t,normal));
    t = normalize(cross(normal,b));
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0;phi<2.0*PI;phi+=sampleDelta){
        for(float theta = 0.0;theta<PI/2.0;theta+=sampleDelta){
            //x y z?
            vec3 tangentSample = vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
            vec3 sampleVec = tangentSample.x * t + tangentSample.y*b + tangentSample.z*normal; 
            irradiance+=texture(environmentMap,sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI*irradiance*(1.0/nrSamples);
    FragColor = vec4(irradiance,1.0);
}