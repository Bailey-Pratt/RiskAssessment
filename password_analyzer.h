#ifndef PASSWORD_ANALYZER_H
#define PASSWORD_ANALYZER_H

#include "common.h"
#include <sys/socket.h>
#include <vector>

// Struct to represent a password check
struct PasswordCheck {
    bool passed;
    std::string description;
};

// Struct to hold password analysis results
struct PasswordAnalysisResult {
    double score;
    std::string rating;
    std::vector<PasswordCheck> checks;
    std::vector<std::string> recommendations;
};

// Function to analyze password
PasswordAnalysisResult analyzePassword(const std::string& password);

// Handler for password analysis endpoint
void handle_analyze_password(const std::string& request, int new_socket);

#endif // PASSWORD_ANALYZER_H