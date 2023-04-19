#version 330 core
//each input variable is known as a vertex attribute
layout(location=0) in vec3 aPos;
//layout(location=1) in vec3 aColor;
layout(location=1) in vec2 aTexCoord;

out vec2 TexCoords;

uniform mat4 model;
layout(std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};
void main()
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	
	gl_Position = projection * view * model * vec4(aPos,1.0);
	TexCoords = aTexCoord;
}