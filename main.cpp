/*#include <mysql/jdbc.h>
#include <iostream>

int main() {
    try {
        // Create a connection
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *conn;

        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", "root", "pass");

        // Connect to the database
        conn->setSchema("RiskDB");

        std::cout << "Connected to MySQL database!" << std::endl;

        // Clean up
        delete conn;
    } catch (sql::SQLException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
*/