#ifndef TSP_REQUEST_H
#define TSP_REQUEST_H

#include "type.h"
#include "response.h"

#include <map>
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cstdlib>

#define new_httpheader(key,value) std::make_pair((key),(value))
#define new_urlarg(ket,value) std::make_pair((key),(value));

tsp_request_t * tsp_request_new(void);
int tsp_request_delete(tsp_request_t * request);
int tsp_request_parse(const std::string& raw_request,tsp_request_t * parse_result);
void tsp_request_handle(int sockfd,tsp_request_t * request);

#endif
