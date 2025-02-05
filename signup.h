#pragma once
#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

class SignupController : public drogon::HttpController<SignupController>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(SignupController::signup, "/signup", drogon::Post);
    METHOD_LIST_END
    
    void signup(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback);
private:
    // Database client pointer
    std::shared_ptr<drogon::orm::DbClient> dbClient;
};
