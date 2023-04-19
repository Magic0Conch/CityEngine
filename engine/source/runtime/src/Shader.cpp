#include "../include/Shader.h"
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;
namespace EasyEngine {
bool Shader::validateShaderSourceByShaderIndex(int shaderIndex,const string type) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderIndex, 512, NULL, infoLog);
		printf("%s",infoLog);
		cout << "ERROR::SHADER::"<<type<<"\n" << infoLog << endl;
		return false;
	}
	return true;
}

unsigned int Shader::readAndCompileShaderByFilename(const char* filename, unsigned int shaderType) {
	//read and compile the vertex shader
	std::string shaderSource = Utility::textFileRead(filename);
	const char* shaderSourceCFormat = shaderSource.c_str();
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSourceCFormat, NULL);
	glCompileShader(shader);
	return shader;
}

unsigned int Shader::loadShaderSourceByFilename(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//read and compile the vertex shader 
	unsigned int vertexShader = readAndCompileShaderByFilename(vertexShaderPath, GL_VERTEX_SHADER);
	validateShaderSourceByShaderIndex(vertexShader,"VERT");
	
	//read and compile the fragment shader
	unsigned int fragmentShader = readAndCompileShaderByFilename(fragmentShaderPath, GL_FRAGMENT_SHADER);
	validateShaderSourceByShaderIndex(fragmentShader,"FRAG");

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// glgetprogramiv
	int success = 0; 
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
		if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s",infoLog);
		cout << "ERROR::SHADER::LINK\n" << infoLog << endl;
		return false;
	}
	//Once linked them into program object, we no longer need them anymore.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int Shader::getVariableLocation(const char* name) const{
	int location = glGetUniformLocation(ID, name);
	if (location == -1)
		printf("ERROR: query %s failed!\n",name);
	return location;
}

int Shader::getVariableLocation(const std::string& name) const{
	int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1)
		printf("ERROR: query %s failed!\n",name.c_str());
	return location;	
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	ID = loadShaderSourceByFilename(vertexPath, fragmentPath);
}

Shader::Shader(const std::string& vertpath,const std::string& fragpath){
	ID = loadShaderSourceByFilename(vertpath.c_str(), fragpath.c_str());
}
Shader::Shader(const std::string& vertpath,const std::string& geompath,const std::string& fragpath){
	//read and compile the vertex shader 
	unsigned int vertexShader = readAndCompileShaderByFilename(vertpath.c_str(), GL_VERTEX_SHADER);

	unsigned int geometryShader = readAndCompileShaderByFilename(geompath.c_str(), GL_GEOMETRY_SHADER);

	//read and compile the fragment shader
	unsigned int fragmentShader = readAndCompileShaderByFilename(fragpath.c_str(), GL_FRAGMENT_SHADER);
	validateShaderSourceByShaderIndex(fragmentShader,"FRAG");
	validateShaderSourceByShaderIndex(vertexShader,"VERT");
	validateShaderSourceByShaderIndex(geometryShader,"GEOM");

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (geometryShader>0) {
		glAttachShader(shaderProgram, geometryShader);
	}
	glLinkProgram(shaderProgram);

	// glgetprogramiv
	int success = 0; 
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
		if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s",infoLog);
		cout << "ERROR::SHADER::LINK\n" << infoLog << endl;
		return;
	}
	//Once linked them into program object, we no longer need them anymore.
	glDeleteShader(vertexShader);
	if (geometryShader>0) {
		glDeleteShader(geometryShader);
	}
	glDeleteShader(fragmentShader);
	ID = shaderProgram;
}

//use/activate shader
void Shader::use() {
	glUseProgram(ID);
}
void Shader::setValue(const char* name,const bool& value){
	glUniform1i(getVariableLocation(name), (int)value);
}

void Shader::setValue(const char* name,const int& value){  
	glUniform1i(getVariableLocation(name), (int)value);

}
void Shader::setValue(const char* name,const float& value){
	glUniform1f(getVariableLocation(name), value);
}

void Shader::setValue(const char* name,const float& value_x,const float& value_y,const float& value_z){
	glUniform3f(getVariableLocation(name),value_x,value_y,value_z);
}
void Shader::setValue(const char* name,const float& value_x,const float& value_y,const float& value_z,const float& value_w){
	glUniform4f(getVariableLocation(name),value_x,value_y,value_z,value_w);
}

void Shader::setValue(const char* name,const glm::mat4& matrix){
	glUniformMatrix4fv(getVariableLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	//the second argument tells OpenGL how many matrices we'd like to send
}

void Shader::setValue(const std::string& name, const glm::vec3& value) {
	glUniform3f(getVariableLocation(name),value.x,value.y,value.z);
}
void Shader::setValue(const std::string& name, const glm::vec2& value) {
	glUniform2f(getVariableLocation(name),value.x,value.y);
}

void Shader::setValue(const std::string& name,const bool& value){
	glUniform1i(getVariableLocation(name), (int)value);
}

void Shader::setValue(const std::string& name,const int& value){
	glUniform1i(getVariableLocation(name), (int)value);

}
void Shader::setValue(const std::string& name,const float& value){
	glUniform1f(getVariableLocation(name), value);
}

void Shader::setValue(const std::string& name,const float& value_x,const float& value_y,const float& value_z){
	glUniform3f(getVariableLocation(name),value_x,value_y,value_z);
}
void Shader::setValue(const std::string& name,const float& value_x,const float& value_y,const float& value_z,const float& value_w){
	glUniform4f(getVariableLocation(name),value_x,value_y,value_z,value_w);
}

void Shader::setValue(const std::string& name,const glm::mat4& matrix){
	glUniformMatrix4fv(getVariableLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	//the second argument tells OpenGL how many matrices we'd like to send
}

}