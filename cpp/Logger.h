#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

// Singleton logger — one instance shared across all threads
class Logger {
private:
    std::ofstream logFile;

    // Private constructor — singleton pattern
    Logger() {
        logFile.open("spas_log.txt", std::ios::app);
    }

public:
    // Returns the single instance of Logger
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    // Log method
    void log(const std::string& message) {
        // Get current timestamp
        time_t now = time(0);
        char   timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

        std::string entry = "[" + std::string(timeStr) + "] " + message;

        // Write to console and log file
        std::cout << entry << std::endl;
        if (logFile.is_open())
            logFile << entry << std::endl;
    }

    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    // Prevent copying — singleton must not be duplicated
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H
