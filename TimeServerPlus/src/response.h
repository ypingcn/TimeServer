#ifndef TSP_RESPONSE_H
#define TSP_RESPONSE_H

#include "type.h"
#include "config.h"
#include "utility.h"
#include "module/load-module.h"

#include <time.h>
#include <sys/time.h>
#include <string>

using std::string;

void tsp_response_headers_end(int sockfd);
void tsp_response_date(int sockfd);
void tsp_response_expires(int sockfd);
void tsp_response_server_name(int sockfd);
void tsp_response_content_length(off_t size,int sockfd);


void tsp_response_ok(int sockfd);
void tsp_response_bad_request(int sockfd);
void tsp_response_forbidden(int sockfd);
void tsp_response_not_found(int sockfd);
void tsp_response_internal_server_error(int sockfd);
void tsp_response_not_implemented(int sockfd);
void tsp_response_http_version_not_supported(int sockfd);

void tsp_response(int sockfd,tsp_request_t * request);
int tsp_response_match(const tsp_request_t * request,const tsp_module_t * module);

#endif
