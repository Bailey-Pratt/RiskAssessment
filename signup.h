#pragma once
#include <drogon/HttpController.h>

class SignupController : public drogon::HttpController<SignupController>
{
public:
    METHOD_LIST_BEGIN
    // Add SignupController:: to specify which class the signup method belongs to
    METHOD_ADD(SignupController::signup, "/signup", drogon::Post);
    METHOD_LIST_END
    
    void signup(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};