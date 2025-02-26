#include "companies_handler.h"
#include <sys/socket.h>

void handle_get_companies(MYSQL* conn, int new_socket) {
    // Query to get the last 5 users
    std::string query = "SELECT username FROM users ORDER BY id DESC LIMIT 5";
    
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result != NULL) {
            // Start building JSON response
            std::string companies_json = "[";
            
            MYSQL_ROW row;
            bool first = true;
            while ((row = mysql_fetch_row(result))) {
                if (!first) {
                    companies_json += ",";
                }
                first = false;
                
                // Escape any special characters in the username
                std::string username = row[0];
                // Replace " with \" for JSON
                size_t pos = 0;
                while ((pos = username.find("\"", pos)) != std::string::npos) {
                    username.replace(pos, 1, "\\\"");
                    pos += 2;
                }
                
                companies_json += "\"" + username + "\"";
            }
            
            companies_json += "]";
            mysql_free_result(result);
            
            // Send response with the companies
            std::string response = RESPONSE_HEADERS + 
                                  "{\"status\":\"success\",\"companies\":" + companies_json + "}";
            send(new_socket, response.c_str(), response.length(), 0);
        } else {
            // Error getting result
            std::string error_response = RESPONSE_HEADERS + 
                                        "{\"status\":\"error\",\"message\":\"Failed to fetch companies\"}";
            send(new_socket, error_response.c_str(), error_response.length(), 0);
        }
    } else {
        // Query failed
        std::string error_response = RESPONSE_HEADERS + 
                                    "{\"status\":\"error\",\"message\":\"Database query failed\"}";
        send(new_socket, error_response.c_str(), error_response.length(), 0);
    }
}