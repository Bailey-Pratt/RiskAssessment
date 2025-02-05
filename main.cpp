#include <drogon/drogon.h>

int main() {
    // Set document root for static files
    drogon::app().setDocumentRoot("/Users/evanlubinsky/Desktop/Risk_Assesment/build/static");
    
    // Add listener
    drogon::app().addListener("0.0.0.0", 3000);
    
    std::cout << "Server starting on port 3000..." << std::endl;
    
    drogon::app().run();
    return 0;
}