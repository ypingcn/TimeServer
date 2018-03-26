#ifndef TSP_TYPE_H
#define TSP_TYPE_H

#include <map>
#include <regex>
#include <string>

typedef std::map<std::string, std::string> tsp_httpheader_t;
typedef std::map<std::string, std::string> tsp_url_arg_t;
typedef struct __tsp_request_t {
  std::string method;
  std::string url;
  std::string version;
  tsp_url_arg_t arg;
  tsp_httpheader_t headers;
  std::string body;
} tsp_request_t;

typedef struct __tsp_module_pattern_t {
  std::regex url_pattern;
  std::string method;
  std::string version;
} tsp_module_pattern_t;

typedef void *(*tsp_module_callback)(void *, void *);

typedef struct __tsp_module_t {
  tsp_module_pattern_t pattern;
  tsp_module_callback callback;
  int priority;
} tsp_module_t;

#define TSP_RN "\r\n"

#define TSP_RESPONSE_OK 200
#define TSP_RESPONSE_BAD_REQUEST 400
#define TSP_RESPONSE_FORBIDDEN 403
#define TSP_RESPONSE_NOT_FOUND 404
#define TSP_RESPONSE_INTERNAL_SERVER_ERROR 500
#define TSP_RESPONSE_NOIMPLEMENTED 501
#define TSP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED 505

#endif
