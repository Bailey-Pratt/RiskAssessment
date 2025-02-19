#include <mysql.h>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Read data sent by the web browser
std::string read_request(SOCKET socket) {
    char buffer[1024] = {0};
    std::string request;
    int bytes_read = recv(socket, buffer, sizeof(buffer), 0);
    if (bytes_read > 0) {
        request.append(buffer, bytes_read);
    }
    return request;
}

// Parse form data - same as before
std::pair<std::string, std::string> parse_form_data(const std::string& request) {
    std::string username, password;
    size_t username_pos = request.find("username=");
    size_t password_pos = request.find("password=");
    
    if (username_pos != std::string::npos) {
        username = request.substr(username_pos + 9);
        size_t end_pos = username.find("&");
        if (end_pos != std::string::npos) {
            username = username.substr(0, end_pos);
        }
    }
    
    if (password_pos != std::string::npos) {
        password = request.substr(password_pos + 9);
        size_t end_pos = password.find("&");
        if (end_pos != std::string::npos) {
            password = password.substr(0, end_pos);
        }
    }
    
    return {username, password};
}

// Store user data - same as before
void store_user_data(MYSQL* conn, const std::string& username, const std::string& password) {
    std::string query = "INSERT INTO users (username, password) VALUES ('" + 
                       username + "', '" + password + "')";
    if (mysql_query(conn, query.c_str()) == 0) {
        std::cout << "Stored user data for: " << username << std::endl;
    } else {
        std::cout << "Error storing user data: " << mysql_error(conn) << std::endl;
    }
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed" << std::endl;
        return 1;
    }

    // MySQL connection initialization
    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", "root", "Lubinsky6", 
                          "riskassesment", 3306, NULL, 0) == NULL) {
        std::cout << "MySQL connection failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Create users table
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS users ("
                     "id INT AUTO_INCREMENT PRIMARY KEY,"
                     "username VARCHAR(50) NOT NULL,"
                     "password VARCHAR(255) NOT NULL,"
                     "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");

    // Create socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cout << "Socket creation failed" << std::endl;
        mysql_close(conn);
        WSACleanup();
        return 1;
    }

    // Set up server address
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cout << "Bind failed" << std::endl;
        closesocket(server_fd);
        mysql_close(conn);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed" << std::endl;
        closesocket(server_fd);
        mysql_close(conn);
        WSACleanup();
        return 1;
    }

    std::cout << "Server running on port 8080\n";

    // CORS headers
    std::string response_headers = "HTTP/1.1 200 OK\n"
                                 "Content-Type: application/json\n"
                                 "Access-Control-Allow-Origin: http://localhost:3000\n"
                                 "Access-Control-Allow-Methods: POST, OPTIONS\n"
                                 "Access-Control-Allow-Headers: Content-Type\n\n";

    // Main server loop
    while(true) {
        sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);
        SOCKET client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        
        if (client_socket == INVALID_SOCKET) {
            std::cout << "Accept failed" << std::endl;
            continue;
        }

        std::string request = read_request(client_socket);
        
        if (request.find("OPTIONS") == 0) {
            send(client_socket, response_headers.c_str(), response_headers.length(), 0);
        } else if (request.find("POST") == 0) {
            auto [username, password] = parse_form_data(request);
            if (!username.empty() && !password.empty()) {
                store_user_data(conn, username, password);
            }
            std::string response = response_headers + "{\"status\":\"success\"}";
            send(client_socket, response.c_str(), response.length(), 0);
        }

        closesocket(client_socket);
    }

    // Cleanup
    closesocket(server_fd);
    mysql_close(conn);
    WSACleanup();
    return 0;
}
