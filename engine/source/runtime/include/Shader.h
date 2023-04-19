//#ifndef SHADER_H
//#define SHADER_H
//#endif 
#pragma once
#include <glad/glad.h>
#include "Utility.h"
#include "glm/fwd.hpp"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace EasyEngine {
class Shader
{
public:
	//shaderProgram ID
	unsigned int ID;

	//read and builds the shader	
	Shader(const char*, const char*);
	Shader(const std::string& vertpath,const std::string& fragpath);
	Shader(const std::string& vertpath,const std::string& geompath,const std::string& fragpath);
	Shader();

	//use/activate shader
	void use();
	void setValue(const char*, const bool& value);
	void setValue(const char*, const int& value);
	void setValue(const char* ,const float& value);
	void setValue(const char*, const float& value_x,const float& value_y,const float& value_z);
	void setValue(const char*, const float&,const float&,const float&,const float&);
	void setValue(const char*, const glm::mat4&);
	void setValue(const std::string&, const glm::vec3&);
	void setValue(const std::string&, const glm::vec2&);
	void setValue(const std::string&, const bool& value);
	void setValue(const std::string&, const int& value);
	void setValue(const std::string&,const float& value);
	void setValue(const std::string&, const float& value_x,const float& value_y,const float& value_z);
	void setValue(const std::string&, const float&,const float&,const float&,const float&);
	void setValue(const std::string&, const glm::mat4&);

private:
	int getVariableLocation(const char*) const;
	int getVariableLocation(const std::string&) const;
	static bool validateShaderSourceByShaderIndex(int shaderIndex,const std::string type);
	static unsigned int readAndCompileShaderByFilename(const char* filename, unsigned int shaderType);
public:
	static unsigned int loadShaderSourceByFilename(const char* vertexShaderPath, const char* fragmentShaderPath);

};

}