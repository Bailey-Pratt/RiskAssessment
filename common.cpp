#include "common.h"
#include <unistd.h>
#include <cstring>

// Helper function to parse form data
std::map<std::string, std::string> parse_form_data(const std::string& body) {
    std::map<std::string, std::string> form_data;
    std::istringstream stream(body);
    std::string pair;
    
    while (std::getline(stream, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            form_data[key] = value;
        }
    }
    
    return form_data;
}

std::string read_complete_request(int socket) {
    char buffer[4096] = {0};
    std::string complete_request;
    
    // Read initial data
    ssize_t bytes_read = read(socket, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        complete_request.append(buffer, bytes_read);
        
        // Find Content-Length
        std::string content_length_header = "Content-Length: ";
        size_t pos = complete_request.find(content_length_header);
        if (pos != std::string::npos) {
            pos += content_length_header.length();
            size_t end_pos = complete_request.find("\r\n", pos);
            int content_length = std::stoi(complete_request.substr(pos, end_pos - pos));
            
            // Find body start
            size_t body_start = complete_request.find("\r\n\r\n");
            if (body_start != std::string::npos) {
                body_start += 4;  // Move past \r\n\r\n
                
                // Read remaining data if needed
                while (complete_request.length() - body_start < content_length) {
                    memset(buffer, 0, sizeof(buffer));
                    bytes_read = read(socket, buffer, sizeof(buffer));
                    if (bytes_read > 0) {
                        complete_request.append(buffer, bytes_read);
                    }
                }
            }
        }
    }
    return complete_request;
}