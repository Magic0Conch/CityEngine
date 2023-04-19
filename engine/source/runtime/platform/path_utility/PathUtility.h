#pragma once
#include <string>
#include <vector>
namespace EasyEngine {
    class PathUtility{        
    public:
        static std::string getRootPath(const std::string&);
        static std::string getFullPath(const std::string& directory,const std::string& filePath);
        static std::vector<std::string> getAllPathWithExt(const std::string& rootPath,const std::string&& extName);
    };
}