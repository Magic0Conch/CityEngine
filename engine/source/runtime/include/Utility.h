#pragma once
#include <cstdio>
#include <atomic>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <unordered_map>
namespace EasyEngine {
	class Utility
	{
	public:
		static std::string textFileRead(const char* fn);
		// static bool dirExists(const std::string& dirName_in);
		// static bool fileExists(const std::string& fileName_in);
		static float* floatArrayRead(const std::string& filePath, int& arrayLength);
		static int* intArrayRead(const std::string& filePath, int& arrayLength);
	};
}