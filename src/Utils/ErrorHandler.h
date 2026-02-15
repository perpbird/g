#pragma once
#include <string>
#include <exception>
#include <sstream>
#include "Logger.h"

class EngineException : public std::exception {
private:
    std::string message;
    std::string function;
    std::string file;
    int line;

public:
    EngineException(const std::string& msg, const std::string& func = "", 
                   const std::string& fil = "", int ln = 0)
        : message(msg), function(func), file(fil), line(ln) {
        Logger::error(toString());
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

    std::string toString() const {
        std::stringstream ss;
        ss << message;
        if (!file.empty()) {
            ss << " [" << file << ":" << line << " in " << function << "]";
        }
        return ss.str();
    }
};

#define THROW_ENGINE_EXCEPTION(msg) \
    throw EngineException(msg, __FUNCTION__, __FILE__, __LINE__)

#define ASSERT_OPENGL(condition) \
    if (!(condition)) { \
        GLenum err = glGetError(); \
        THROW_ENGINE_EXCEPTION("OpenGL error: " + std::to_string(err)); \
    }
    