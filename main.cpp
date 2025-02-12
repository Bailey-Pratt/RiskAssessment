#include <mysql.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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

    // Verify users table exists
    if (mysql_query(conn, "SHOW TABLES LIKE 'users'") == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (mysql_num_rows(result) > 0) {
            std::cout << "Users table exists" << std::endl;
        } else {
            std::cout << "Creating users table..." << std::endl;
            mysql_query(conn, "CREATE TABLE IF NOT EXISTS users ("
                            "id INT AUTO_INCREMENT PRIMARY KEY,"
                            "username VARCHAR(50) NOT NULL,"
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

    char buffer[4096] = {0};
    std::string html_content = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
                              "<html><body>"
                              "<form method='post'>"
                              "<input type='text' name='username'>"
                              "<input type='submit'>"
                              "</form>"
                              "</body></html>";

    while(true) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        
        // Replace the single read with complete request reading
        std::string request = read_complete_request(new_socket);
        
        if (request.find("POST") == 0) {
            std::cout << "\nFull request:\n" << request << "\nEnd of request\n";
            handle_post(conn, request);
        }

        send(new_socket, html_content.c_str(), html_content.length(), 0);
        close(new_socket);
    }

    mysql_close(conn);
    close(server_fd);
    return 0;
}
