#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Logger.h"

class FileUtils {
public:
    static std::string readFile(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::in);
        if (!file.is_open()) {
            Logger::error("Failed to open file: " + filepath);
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        Logger::debug("File loaded: " + filepath + " (" + 
                     std::to_string(buffer.str().size()) + " bytes)");
        return buffer.str();
    }

    static bool fileExists(const std::string& filepath) {
        std::ifstream file(filepath);
        return file.good();
    }

    static std::string getFileExtension(const std::string& filepath) {
        size_t pos = filepath.find_last_of('.');
        if (pos != std::string::npos) {
            return filepath.substr(pos + 1);
        }
        return "";
    }

    static std::string getFileName(const std::string& filepath) {
        size_t pos = filepath.find_last_of("/\\");
        if (pos != std::string::npos) {
            return filepath.substr(pos + 1);
        }
        return filepath;
    }
};