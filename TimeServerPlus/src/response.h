#ifndef TSP_RESPONSE_H
#define TSP_RESPONSE_H

#include "config.h"
#include "module/load-module.h"
#include "request.h"
#include "type.h"
#include "utility.h"

#include <sys/time.h>
#include <time.h>

void tsp_response_headers_end(int sockfd);
void tsp_response_date(int sockfd);
void tsp_response_expires(int sockfd);
void tsp_response_server_name(int sockfd);
void tsp_response_content_length(off_t size, int sockfd);

void tsp_response_ok(int sockfd);
void tsp_response_bad_request(int sockfd);
void tsp_response_forbidden(int sockfd);
void tsp_response_not_found(int sockfd);
void tsp_response_internal_server_error(int sockfd);
void tsp_response_not_implemented(int sockfd);
void tsp_response_http_version_not_supported(int sockfd);

typedef struct _TSPResponsePatternType {
  regex url_pattern;
  string method;
  string version;
} TSPResponsePatternType;

class TSPRequest;

#define MAX_RESPONSE_PRIORITY 200
#define DEFAULT_RESPONSE_PRIORITY 100
#define MIN_RESPONSE_PRIORITY 0

class TSPBasicResponse {
public:
  explicit TSPBasicResponse(int _priority, TSPResponsePatternType _pattern)
      : priority(_priority), pattern(_pattern){};
  virtual ~TSPBasicResponse() = default;
  TSPBasicResponse(const TSPBasicResponse &) = default;
  TSPBasicResponse &operator=(const TSPBasicResponse &) = default;

  int get_priority() { return priority; }
  int match(const TSPRequest &req);
  virtual int handle(const TSPRequest &req) = 0;

private:
  int priority = DEFAULT_RESPONSE_PRIORITY;
  TSPResponsePatternType pattern;
};

void tsp_response(const TSPRequest &req);

#endif
