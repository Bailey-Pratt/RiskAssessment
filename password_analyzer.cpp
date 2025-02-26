#include "password_analyzer.h"
#include <sys/socket.h>
#include <algorithm>
#include <cctype>
#include <regex>
#include <set>
#include <string>
#include <sstream>

PasswordAnalysisResult analyzePassword(const std::string& password) {
    PasswordAnalysisResult result;
    
    // Initialize score
    double score = 0.0;
    
    // Check 1: Length
    bool hasGoodLength = password.length() >= 8;
    score += hasGoodLength ? 2.0 : 0.0;
    score += std::min(2.0, password.length() / 10.0); // Up to 2 points for length
    
    result.checks.push_back({
        hasGoodLength,
        "Password is at least 8 characters long"
    });
    
    // Check 2: Contains uppercase
    bool hasUppercase = std::any_of(password.begin(), password.end(), [](char c) { return std::isupper(c); });
    score += hasUppercase ? 1.5 : 0.0;
    
    result.checks.push_back({
        hasUppercase,
        "Contains uppercase letters"
    });
    
    // Check 3: Contains lowercase
    bool hasLowercase = std::any_of(password.begin(), password.end(), [](char c) { return std::islower(c); });
    score += hasLowercase ? 1.0 : 0.0;
    
    result.checks.push_back({
        hasLowercase,
        "Contains lowercase letters"
    });
    
    // Check 4: Contains digits
    bool hasDigits = std::any_of(password.begin(), password.end(), [](char c) { return std::isdigit(c); });
    score += hasDigits ? 1.5 : 0.0;
    
    result.checks.push_back({
        hasDigits,
        "Contains numeric characters"
    });
    
    // Check 5: Contains special characters
    bool hasSpecial = std::any_of(password.begin(), password.end(), [](char c) { 
        return !std::isalnum(c); 
    });
    score += hasSpecial ? 2.0 : 0.0;
    
    result.checks.push_back({
        hasSpecial,
        "Contains special characters"
    });
    
    // Check 6: Character variety (using set to count unique characters)
    std::set<char> uniqueChars(password.begin(), password.end());
    double varietyScore = uniqueChars.size() / static_cast<double>(password.length());
    score += varietyScore * 2.0; // Up to 2 points for variety
    
    result.checks.push_back({
        varietyScore > 0.5,
        "Has good character variety"
    });
    
    // Cap the score at 10
    score = std::min(10.0, score);
    
    // Set the rating
    if (score < 4.0) {
        result.rating = "weak";
    } else if (score < 7.0) {
        result.rating = "medium";
    } else {
        result.rating = "strong";
    }
    
    // Add recommendations based on failed checks
    result.recommendations.clear();
    if (!hasGoodLength) {
        result.recommendations.push_back("Use at least 8 characters for your password");
    }
    if (!hasUppercase) {
        result.recommendations.push_back("Add uppercase letters to your password");
    }
    if (!hasLowercase) {
        result.recommendations.push_back("Add lowercase letters to your password");
    }
    if (!hasDigits) {
        result.recommendations.push_back("Add numbers to your password");
    }
    if (!hasSpecial) {
        result.recommendations.push_back("Add special characters like !@#$%^&*");
    }
    if (varietyScore <= 0.5) {
        result.recommendations.push_back("Use a greater variety of characters");
    }
    
    // Set the final score
    result.score = std::round(score * 10) / 10; // Round to 1 decimal place
    
    return result;
}

void handle_analyze_password(const std::string& request, int new_socket) {
    // Find the end of headers (double newline)
    size_t header_end = request.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        // Get the body of the request
        std::string body = request.substr(header_end + 4);
        
        // Find the password field in the JSON
        size_t password_pos = body.find("\"password\"");
        if (password_pos != std::string::npos) {
            // Extract the password value
            size_t value_start = body.find(":", password_pos) + 1;
            // Skip whitespace
            while (value_start < body.length() && std::isspace(body[value_start])) {
                value_start++;
            }
            
            // Check if the value is surrounded by quotes
            bool has_quotes = body[value_start] == '"';
            size_t content_start = has_quotes ? value_start + 1 : value_start;
            
            size_t content_end;
            if (has_quotes) {
                content_end = body.find("\"", content_start);
            } else {
                // Find the end of the value (comma, closing brace, or end of string)
                size_t comma_pos = body.find(",", content_start);
                size_t brace_pos = body.find("}", content_start);
                content_end = std::min(comma_pos, brace_pos);
                if (content_end == std::string::npos) {
                    content_end = body.length();
                }
            }
            
            if (content_end != std::string::npos) {
                std::string password = body.substr(content_start, content_end - content_start);
                
                // Analyze the password
                PasswordAnalysisResult analysis = analyzePassword(password);
                
                // Create JSON response
                std::stringstream json;
                json << "{";
                json << "\"score\":" << analysis.score * 10 << ","; // Convert to scale of 100
                json << "\"rating\":\"" << analysis.rating << "\",";
                
                // Add checks array
                json << "\"checks\":[";
                for (size_t i = 0; i < analysis.checks.size(); ++i) {
                    if (i > 0) json << ",";
                    json << "{";
                    json << "\"passed\":" << (analysis.checks[i].passed ? "true" : "false") << ",";
                    json << "\"description\":\"" << analysis.checks[i].description << "\"";
                    json << "}";
                }
                json << "],";
                
                // Add recommendations array
                json << "\"recommendations\":[";
                for (size_t i = 0; i < analysis.recommendations.size(); ++i) {
                    if (i > 0) json << ",";
                    json << "\"" << analysis.recommendations[i] << "\"";
                }
                json << "]";
                
                json << "}";
                
                // Send the response
                std::string response = RESPONSE_HEADERS + json.str();
                send(new_socket, response.c_str(), response.length(), 0);
                return;
            }
        }
    }
    
    // If we get here, something went wrong with the request
    std::string error_response = RESPONSE_HEADERS + "{\"error\":\"Invalid request format\"}";
    send(new_socket, error_response.c_str(), error_response.length(), 0);
}