#include "signup.h"

void SignupController::signup(const drogon::HttpRequestPtr& req,
                            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
    // Simple response without database
    auto resp = drogon::HttpResponse::newHttpJsonResponse(Json::Value("Received signup request"));
    callback(resp);
}