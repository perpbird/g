#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    static std::ofstream logFile;
    static bool consoleOutput;
    static LogLevel minLevel;

public:
    static void init(const std::string& filename = "engine.log", bool console = true) {
        if (logFile.is_open()) logFile.close();
        logFile.open(filename);
        consoleOutput = console;
    }

    static void setMinLevel(LogLevel level) { minLevel = level; }

    static void log(LogLevel level, const std::string& message) {
        if (level < minLevel) return;

        std::string levelStr;
        switch (level) {
            case LogLevel::DEBUG:   levelStr = "DEBUG"; break;
            case LogLevel::INFO:    levelStr = "INFO"; break;
            case LogLevel::WARNING: levelStr = "WARNING"; break;
            case LogLevel::ERROR:   levelStr = "ERROR"; break;
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::string timeStr = std::ctime(&time);
        timeStr.pop_back(); // убираем \n

        std::string logMsg = "[" + timeStr + "] [" + levelStr + "] " + message;

        if (consoleOutput) {
            if (level == LogLevel::ERROR)
                std::cerr << logMsg << std::endl;
            else
                std::cout << logMsg << std::endl;
        }

        if (logFile.is_open()) {
            logFile << logMsg << std::endl;
            logFile.flush();
        }
    }

    static void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    static void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    static void warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
    static void error(const std::string& msg) { log(LogLevel::ERROR, msg); }

    static void shutdown() {
        if (logFile.is_open()) logFile.close();
    }
};