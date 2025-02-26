#ifndef COMPANIES_HANDLER_H
#define COMPANIES_HANDLER_H

#include "common.h"
#include <sys/socket.h>

void handle_get_companies(MYSQL* conn, int new_socket);

#endif // COMPANIES_HANDLER_H