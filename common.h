#ifndef COMMON_H
#define COMMON_H

#include <mysql.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

// In common.h
std::map<std::string, std::string> parse_form_data(const std::string& body);

// Common HTTP headers
const std::string RESPONSE_HEADERS = "HTTP/1.1 200 OK\n"
                         "Content-Type: application/json\n"
                         "Access-Control-Allow-Origin: http://localhost:3000\n"
                         "Access-Control-Allow-Methods: POST, GET, OPTIONS\n"
                         "Access-Control-Allow-Headers: Content-Type\n\n";

// Function to read a complete HTTP request
std::string read_complete_request(int socket);

#endif // COMMON_H