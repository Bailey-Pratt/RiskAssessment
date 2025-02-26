#include "post_handler.h"

void handle_post(MYSQL* conn, const std::string& request) {
    // Find the end of headers (double newline)
    size_t header_end = request.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        // Get the body of the request
        std::string body = request.substr(header_end + 4);
        std::cout << "Request body: " << body << std::endl;

        // Parse username from body
        size_t pos = body.find("username=");
        if (pos != std::string::npos) {
            std::string username = body.substr(pos + 9);
            std::cout << "Found username: " << username << std::endl;
            
            // Create and execute query
            std::string query = "INSERT INTO users (username) VALUES ('" + username + "')";
            if (mysql_query(conn, query.c_str()) == 0) {
                std::cout << "Successfully stored username: " << username << std::endl;
                
                // Verify the insertion
                if (mysql_query(conn, "SELECT * FROM users ORDER BY id DESC LIMIT 1") == 0) {
                    MYSQL_RES *result = mysql_store_result(conn);
                    if (result != NULL) {
                        MYSQL_ROW row = mysql_fetch_row(result);
                        if (row != NULL) {
                            std::cout << "Last inserted username from DB: " << row[1] << std::endl;
                        }
                        mysql_free_result(result);
                    }
                }
            } else {
                std::cout << "Database error: " << mysql_error(conn) << std::endl;
            }
        }
    }
}