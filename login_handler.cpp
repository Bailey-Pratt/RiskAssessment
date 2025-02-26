#include "login_handler.h"
#include <sys/socket.h>

void handle_login(MYSQL* conn, const std::string& request, int new_socket) {
    // Find the end of headers (double newline)
    size_t header_end = request.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        // Get the body of the request
        std::string body = request.substr(header_end + 4);
        std::cout << "Login request body: " << body << std::endl;

        // Parse form data
        auto form_data = parse_form_data(body);
        
        if (form_data.count("username") && form_data.count("password")) {
            std::string username = form_data["username"];
            std::string password = form_data["password"];
            
            std::cout << "Login attempt for user: " << username << std::endl;
            
            // Check credentials
            std::string query = "SELECT id FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
            if (mysql_query(conn, query.c_str()) == 0) {
                MYSQL_RES *result = mysql_store_result(conn);
                if (result != NULL) {
                    if (mysql_num_rows(result) > 0) {
                        MYSQL_ROW row = mysql_fetch_row(result);
                        std::string user_id = row[0];
                        mysql_free_result(result);
                        
                        std::string success_response = RESPONSE_HEADERS + 
                            "{\"status\":\"success\",\"message\":\"Login successful\",\"userId\":\"" + user_id + "\"}";
                        send(new_socket, success_response.c_str(), success_response.length(), 0);
                        return;
                    }
                    mysql_free_result(result);
                }
            }
            
            // Invalid credentials
            std::string error_response = RESPONSE_HEADERS + 
                "{\"status\":\"error\",\"message\":\"Invalid username or password\"}";
            send(new_socket, error_response.c_str(), error_response.length(), 0);
        } else {
            std::string error_response = RESPONSE_HEADERS + 
                "{\"status\":\"error\",\"message\":\"Missing required fields\"}";
            send(new_socket, error_response.c_str(), error_response.length(), 0);
        }
    }
}