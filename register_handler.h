#ifndef REGISTER_HANDLER_H
#define REGISTER_HANDLER_H

#include "common.h"
#include <sys/socket.h>

void handle_register(MYSQL* conn, const std::string& request, int new_socket);

#endif // REGISTER_HANDLER_H