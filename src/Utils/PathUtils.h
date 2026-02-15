#pragma once
#include <string>
#include <libgen.h>  // для dirname
#include <unistd.h>  // для readlink
#include <linux/limits.h>  // для PATH_MAX

class PathUtils {
public:
    // Получить путь к директории, где находится исполняемый файл
    static std::string getExecutablePath() {
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            result[count] = '\0';
            return std::string(dirname(result));
        }
        return ".";
    }
    
    // Получить путь к ресурсам (рядом с исполняемым файлом)
    static std::string getResourcesPath() {
        return getExecutablePath() + "/res/";
    }
};