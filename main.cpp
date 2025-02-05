#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>

int main() {
    try {
        // Connection string
        std::string connStr = "postgresql://evanlubinsky@localhost:5433/postgres";
        
        // Create database client with required number of connections
        auto dbClient = drogon::orm::DbClient::newPgClient(connStr, 1);
        
        // Create static file directory if it doesn't exist
        std::string staticPath = "/Users/evanlubinsky/Desktop/Risk_Assesment/build/static";
        if (!std::filesystem::exists(staticPath)) {
            std::filesystem::create_directories(staticPath);
            std::cout << "Created static directory at: " << staticPath << std::endl;
        }
        
        // Set document root for static files
        drogon::app().setDocumentRoot(staticPath);
        
        // Enable debugging
        drogon::app().setLogLevel(trantor::Logger::kDebug);
        
        // Add listener
        drogon::app().addListener("0.0.0.0", 3000);
        
        std::cout << "Server starting on port 3000..." << std::endl;
        std::cout << "Document root set to: " << staticPath << std::endl;
        
        drogon::app().run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
