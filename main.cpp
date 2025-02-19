#include <mysql.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


// this reads the data that is sent to us by the web browser  
// Returns the data as a string so it's easier to work with
std::string read_request(int socket) {
    char buffer[1024] = {0}; // Create empty buffer to store incoming data
    std::string request;
    ssize_t bytes_read = read(socket, buffer, sizeof(buffer));
    if (bytes_read > 0) {
                // Only copy the amount of data we actually received
        request.append(buffer, bytes_read);
    }
    return request;
}

// Function to extract username and password from the form data
// Form data comes in format: "username=test&password=123"
// Returns both username and password as a pair
std::pair<std::string, std::string> parse_form_data(const std::string& request) 
{
    std::string username, password;
    size_t username_pos = request.find("username=");
    size_t password_pos = request.find("password=");
    
    // Get username if we found it
    if (username_pos != std::string::npos) {
  username = request.substr(username_pos + 9);
        size_t end_pos = username.find("&");
      if (end_pos != std::string::npos) {
            username = username.substr(0, end_pos);
        }
    }
    
    // same as username
    if (password_pos != std::string::npos) {
        password = request.substr(password_pos + 9);
  size_t end_pos = password.find("&");
        if (end_pos != std::string::npos) {
            password = password.substr(0, end_pos);
        }
    }
    
    return {username, password};
}
// Function to save user information to MySQL database
// Right now it is vunlerable to sql injection.
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
    // MySQL connection initialization with explicit credentials
    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", "root", "Lubinsky6", 
                          "riskassesment", 3306, NULL, 0) == NULL) {
        std::cout << "MySQL connection failed" << std::endl;
        return 1;
    }

    // Create users table with password field
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS users ("
                     "id INT AUTO_INCREMENT PRIMARY KEY,"
                     "username VARCHAR(50) NOT NULL,"
                     "password VARCHAR(255) NOT NULL,"
                     "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
// Set up the network connection (socket)
    // This lets the server accept connections from web browsers

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from anywhere
    address.sin_port = htons(8080); // use port 8080 

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    std::cout << "Server running on port 8080\n";
// CORS (Cross-Origin Resource Sharing) implementation
// These headers are needed so the web browser will accept our responses
    std::string response_headers = "HTTP/1.1 200 OK\n"
                                 "Content-Type: application/json\n"
                                 "Access-Control-Allow-Origin: http://localhost:3000\n"
                                 "Access-Control-Allow-Methods: POST, OPTIONS\n"
                                 "Access-Control-Allow-Headers: Content-Type\n\n";
 // Main server loop - keep running forever and handle requests
    while(true) {
        socklen_t addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        
        std::string request = read_request(client_socket);
        // Browser sends this to make sure it's allowed to talk to our server
        if (request.find("OPTIONS") == 0) {
            send(client_socket, response_headers.c_str(), response_headers.length(), 0);
                // Handle actual registration requests (POST requests)

        } else if (request.find("POST") == 0) {
            auto [username, password] = parse_form_data(request);
            if (!username.empty() && !password.empty()) {
                store_user_data(conn, username, password);
            }
            std::string response = response_headers + "{\"status\":\"success\"}";
            send(client_socket, response.c_str(), response.length(), 0);
        }

        close(client_socket);
    }

    mysql_close(conn);
    close(server_fd);
    return 0;
}
