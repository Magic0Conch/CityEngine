#pragma once
#include <string>
#include <json/json.h>
#include <iostream>
#include <fstream>
namespace EasyEngine {
class JsonManipulation
{
private:
	Json::Value jsonData;
	Json::Reader reader;
	std::string jsonFilePath="";
private:
	void updateJsonData();
public:

	JsonManipulation(const std::string& jsonFilePath);
	JsonManipulation();
	virtual ~JsonManipulation();
	Json::Value& getJsonData(const std::string& jsonFilePath = "");
	void setJsonFilePath(const std::string& jsonFilePath);
};

}