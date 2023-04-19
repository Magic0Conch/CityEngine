#version 330 core
layout(points) in;
layout(triangle_strip,max_vertices=5) out;

in VS_OUT{
    vec3 Color;
}gs_in[];
out vec3 fColor;
void build_house(){
    fColor = gs_in[0].Color;
    gl_Position = gl_in[0].gl_Position+vec4(-0.2,-0.2,0.0,0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(0.2,-0.2,0.0,0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(-0.2,0.2,0.0,0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(0.2,0.2,0.0,0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(0,0.4,0.0,0.0);
    fColor = vec3(1,1,1);
    EmitVertex();
    EndPrimitive();
}

void main(){
    build_house();
}