#ifndef TSP_TYPE_H
#define TSP_TYPE_H

#include <iostream>
#include <map>
#include <regex>
#include <string>

using std::cout;
using std::endl;
using std::regex;
using std::string;

#define MAX_BACKLOG 200
#define MAX_EVENT_SIZE 1024
#define KILO 1024
#define BUFF_SIZE 1024 * KILO

#define TSP_RN "\r\n"

#define TSP_RESPONSE_OK 200
#define TSP_RESPONSE_BAD_REQUEST 400
#define TSP_RESPONSE_FORBIDDEN 403
#define TSP_RESPONSE_NOT_FOUND 404
#define TSP_RESPONSE_INTERNAL_SERVER_ERROR 500
#define TSP_RESPONSE_NOIMPLEMENTED 501
#define TSP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED 505

#endif
