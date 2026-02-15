#include "Logger.h"

// Определение статических членов
std::ofstream Logger::logFile;
bool Logger::consoleOutput = true;
LogLevel Logger::minLevel = LogLevel::INFO;