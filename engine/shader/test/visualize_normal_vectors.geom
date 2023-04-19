#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


in VS_OUT{
    vec3 normal;
    mat4 projection;
}gs_in[];

out vec2 TexCoords;


const float MAGNITUDE = 0.4;

void GenerateLine(int index){
    gl_Position = gs_in[0].projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gs_in[0].projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal*MAGNITUDE,0.0));
    EmitVertex();
    EndPrimitive();
}

void main(){
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}