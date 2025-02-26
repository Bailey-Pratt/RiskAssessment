#include "register_handler.h"
#include <sys/socket.h>

void handle_register(MYSQL* conn, const std::string& request, int new_socket) {
    // Find the end of headers (double newline)
    size_t header_end = request.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        // Get the body of the request
        std::string body = request.substr(header_end + 4);
        std::cout << "Register request body: " << body << std::endl;

        // Parse form data
        auto form_data = parse_form_data(body);
        
        if (form_data.count("username") && form_data.count("password") && form_data.count("email")) {
            std::string username = form_data["username"];
            std::string password = form_data["password"];
            std::string email = form_data["email"];
            
            std::cout << "Registering user: " << username << std::endl;
            
            // Check if username already exists
            std::string check_query = "SELECT * FROM users WHERE username = '" + username + "'";
            if (mysql_query(conn, check_query.c_str()) == 0) {
                MYSQL_RES *result = mysql_store_result(conn);
                if (result != NULL) {
                    if (mysql_num_rows(result) > 0) {
                        // Username already exists
                        mysql_free_result(result);
                        std::string error_response = RESPONSE_HEADERS + 
                            "{\"status\":\"error\",\"message\":\"Username already exists\"}";
                        send(new_socket, error_response.c_str(), error_response.length(), 0);
                        return;
                    }
                    mysql_free_result(result);
                }
            }
            
            // Create and execute query for new user
            std::string query = "INSERT INTO users (username, password, email) VALUES ('" 
                            + username + "', '" + password + "', '" + email + "')";
            
            if (mysql_query(conn, query.c_str()) == 0) {
                std::cout << "Successfully registered user: " << username << std::endl;
                
                std::string success_response = RESPONSE_HEADERS + 
                    "{\"status\":\"success\",\"message\":\"Account created successfully\"}";
                send(new_socket, success_response.c_str(), success_response.length(), 0);
            } else {
                std::cout << "Database error: " << mysql_error(conn) << std::endl;
                std::string error_response = RESPONSE_HEADERS + 
                    "{\"status\":\"error\",\"message\":\"Database error\"}";
                send(new_socket, error_response.c_str(), error_response.length(), 0);
            }
        } else {
            std::string error_response = RESPONSE_HEADERS + 
                "{\"status\":\"error\",\"message\":\"Missing required fields\"}";
            send(new_socket, error_response.c_str(), error_response.length(), 0);
        }
    }
}