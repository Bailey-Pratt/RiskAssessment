#include <mysql.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Simple function to read HTTP request
std::string read_request(int socket) {
    char buffer[1024] = {0};
    std::string request;
    ssize_t bytes_read = read(socket, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        request.append(buffer, bytes_read);
    }
    return request;
}

// Handle POST request to store username
void store_username(MYSQL* conn, const std::string& request) {
    size_t pos = request.find("username=");
    if (pos != std::string::npos) {
        std::string username = request.substr(pos + 9);
        // Remove any trailing data after the username
        size_t end_pos = username.find("&");
        if (end_pos != std::string::npos) {
            username = username.substr(0, end_pos);
        }
        
        // Create and execute query
        std::string query = "INSERT INTO users (username) VALUES ('" + username + "')";
        if (mysql_query(conn, query.c_str()) == 0) {
            std::cout << "Stored username: " << username << std::endl;
        } else {
            std::cout << "Error storing username: " << mysql_error(conn) << std::endl;
        }
    }
}

int main() {
    // Initialize MySQL connection
    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", "root", "Lubinsky6", 
                          "riskassesment", 3306, NULL, 0) == NULL) {
        std::cout << "MySQL connection failed" << std::endl;
        return 1;
    }

    // Create users table if it doesn't exist
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS users ("
                     "id INT AUTO_INCREMENT PRIMARY KEY,"
                     "username VARCHAR(50) NOT NULL,"
                     "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");

    // Set up server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    std::cout << "Server running on port 8080\n";

    // CORS headers for response
    std::string response_headers = "HTTP/1.1 200 OK\n"
                                 "Content-Type: application/json\n"
                                 "Access-Control-Allow-Origin: http://localhost:3000\n"
                                 "Access-Control-Allow-Methods: POST, OPTIONS\n"
                                 "Access-Control-Allow-Headers: Content-Type\n\n";

    // Main server loop
    while(true) {
        socklen_t addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        
        std::string request = read_request(client_socket);
        
        if (request.find("OPTIONS") == 0) {
            // Handle CORS preflight
            send(client_socket, response_headers.c_str(), response_headers.length(), 0);
        } else if (request.find("POST") == 0) {
            // Handle username submission
            store_username(conn, request);
            std::string response = response_headers + "{\"status\":\"success\"}";
            send(client_socket, response.c_str(), response.length(), 0);
        }

        close(client_socket);
    }

    mysql_close(conn);
    close(server_fd);
    return 0;
}
