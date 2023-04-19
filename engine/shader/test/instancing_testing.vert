#version 330 core
layout(location = 0)in vec2 aPos;
layout(location = 1)in vec3 aColor;
layout(location = 2)in vec2 aOffset;
out vec3 Color;
// uniform vec2 offsets[100];
void main(){
    // vec2 offset = offsets[gl_InstanceID];
    vec2 pos = aPos*gl_InstanceID/100;
    gl_Position = vec4(pos+aOffset,0,1);
    Color = aColor;
}