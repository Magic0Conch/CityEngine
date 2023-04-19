#include "PathUtility.h"
#include <algorithm>
#include <iostream>
#include <runtime/core/base/macro.h>
#include <string>
#include <filesystem>
#include <vector>

namespace EasyEngine{
    
    std::string PathUtility::getRootPath(const std::string& path){
        auto platform = getPlatformName();
        int index = path.find_last_of('/');
        if (platform == PlatformNames::windows) {
            index = path.find_last_of('\\');
        }
        std::string rootPath = path.substr(0,index+1);
        
        return rootPath;
    }

    std::string PathUtility::getFullPath(const std::string &directory, const std::string &filePath){
        auto platform = getPlatformName();
        std::string dir = directory,file = filePath;
        if (platform == PlatformNames::windows) {
            std::replace(dir.begin(),dir.end(),'/','\\');
            std::replace(file.begin(),file.end(),'/','\\');
        }
        else if(platform == PlatformNames::linux){
            std::replace(dir.begin(),dir.end(),'\\','/');
            std::replace(file.begin(),file.end(),'\\','/');
        }
        
        std::string fullPath;
        fullPath = dir;

        if (platform == PlatformNames::windows) {
            if (dir.back()!='\\') {
                fullPath.push_back('\\');
            }
        }
        else if (platform == PlatformNames::linux) {
            if (dir.back()!='/') {
                fullPath.push_back('/');
            }
        }
        fullPath.append(file);
        return fullPath;
    }

    std::vector<std::string> PathUtility::getAllPathWithExt(const std::string& rootPath,const std::string&& extName){
        std::vector<std::string> paths;
        using namespace std::filesystem;
        for (const auto &entry: recursive_directory_iterator(rootPath)) {
            if(entry.path().extension()==extName){
                paths.emplace_back(entry.path().string());
                // std::cout<<paths.back()<<std::endl;
            }
        }
        return paths;
    }
}