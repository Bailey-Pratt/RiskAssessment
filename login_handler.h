#ifndef LOGIN_HANDLER_H
#define LOGIN_HANDLER_H

#include "common.h"
#include <sys/socket.h>

void handle_login(MYSQL* conn, const std::string& request, int new_socket);

#endif // LOGIN_HANDLER_H