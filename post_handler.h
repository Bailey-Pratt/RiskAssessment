#ifndef POST_HANDLER_H
#define POST_HANDLER_H

#include "common.h"

void handle_post(MYSQL* conn, const std::string& request);

#endif // POST_HANDLER_H