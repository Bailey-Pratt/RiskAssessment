#include "common.h"
#include "register_handler.h"
#include "login_handler.h"
#include "post_handler.h"
#include "companies_handler.h"
#include "password_analyzer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cout << "MySQL init failed" << std::endl;
        return 1;
    }

    if (mysql_real_connect(conn, "localhost", "root", "Lubinsky6", 
                          "riskassesment", 3306, NULL, 0) == NULL) {
        std::cout << "Connection Error: " << mysql_error(conn) << std::endl;
        return 1;
    }
    std::cout << "Successfully connected to MySQL database!" << std::endl;

    // Verify database connection and version
    if (mysql_query(conn, "SELECT VERSION()") == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                std::cout << "MySQL Version: " << row[0] << std::endl;
            }
            mysql_free_result(result);
        }
    }

    // Verify users table exists with correct structure
    if (mysql_query(conn, "SHOW TABLES LIKE 'users'") == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (mysql_num_rows(result) > 0) {
            std::cout << "Users table exists" << std::endl;
            
            // Check if we need to update the table structure
            if (mysql_query(conn, "SHOW COLUMNS FROM users") == 0) {
                MYSQL_RES *columns_result = mysql_store_result(conn);
                bool has_password = false;
                bool has_email = false;
                
                MYSQL_ROW column_row;
                while ((column_row = mysql_fetch_row(columns_result))) {
                    if (std::string(column_row[0]) == "password") {
                        has_password = true;
                    }
                    if (std::string(column_row[0]) == "email") {
                        has_email = true;
                    }
                }
                
                mysql_free_result(columns_result);
                
                if (!has_password || !has_email) {
                    std::cout << "Updating users table structure..." << std::endl;
                    if (!has_password) {
                        mysql_query(conn, "ALTER TABLE users ADD COLUMN password VARCHAR(100) NOT NULL DEFAULT ''");
                    }
                    if (!has_email) {
                        mysql_query(conn, "ALTER TABLE users ADD COLUMN email VARCHAR(100) NOT NULL DEFAULT ''");
                    }
                }
            }
        } else {
            std::cout << "Creating users table..." << std::endl;
            mysql_query(conn, "CREATE TABLE IF NOT EXISTS users ("
                            "id INT AUTO_INCREMENT PRIMARY KEY,"
                            "username VARCHAR(50) NOT NULL,"
                            "password VARCHAR(100) NOT NULL,"
                            "email VARCHAR(100) NOT NULL,"
                            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
        }
        mysql_free_result(result);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    std::cout << "Server running on port 8080\n";

    std::string success_response = RESPONSE_HEADERS + "{\"status\":\"success\"}";

    while(true) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        
        // Read the complete request
        std::string request = read_complete_request(new_socket);
        
        if (request.find("OPTIONS") == 0) {
            // Handle preflight requests
            send(new_socket, RESPONSE_HEADERS.c_str(), RESPONSE_HEADERS.length(), 0);
        } else if (request.find("GET /companies") == 0) {
            // Handle get companies request
            handle_get_companies(conn, new_socket);
        } else if (request.find("POST /register") == 0) {
            // Handle registration
            handle_register(conn, request, new_socket);
        } else if (request.find("POST /login") == 0) {
            // Handle login
            handle_login(conn, request, new_socket);
        } else if (request.find("POST /analyze-password") == 0) {
            // Handle password analysis
            handle_analyze_password(request, new_socket);
        } else if (request.find("POST") == 0) {
            // Handle other POST requests
            std::cout << "\nFull request:\n" << request << "\nEnd of request\n";
            handle_post(conn, request);
            send(new_socket, success_response.c_str(), success_response.length(), 0);
        } else {
            // Handle other requests
            send(new_socket, success_response.c_str(), success_response.length(), 0);
        }

        close(new_socket);
    }

    mysql_close(conn);
    close(server_fd);
    return 0;
}
