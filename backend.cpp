#include "signup.h"
#include <drogon/orm/Exception.h>

void SignupController::signup(const drogon::HttpRequestPtr& req,
                            std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
    auto json = req->getJsonObject();
    if (!json) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            Json::Value("Invalid JSON"));
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string username;
    if (!json->isMember("username") || !(*json)["username"].isString()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            Json::Value("Username required"));
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    
    username = (*json)["username"].asString();
    
    // Get database client
    dbClient = drogon::app().getDbClient();
    
    try {
        // Insert user into database
        dbClient->execSqlAsync(
            "INSERT INTO users (username, created_at) VALUES ($1, NOW())",
            [callback](const drogon::orm::Result &result) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    Json::Value("User created successfully"));
                callback(resp);
            },
            [callback](const drogon::orm::DrogonDbException &e) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    Json::Value(e.base().what()));
                resp->setStatusCode(drogon::k500InternalServerError);
                callback(resp);
            },
            username
        );
    } catch (const drogon::orm::DrogonDbException &e) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            Json::Value(e.base().what()));
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }
}
