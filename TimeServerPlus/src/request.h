#ifndef TSP_REQUEST_H
#define TSP_REQUEST_H

#include "response.h"
#include "type.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#define new_httpheader(key, value) std::make_pair((key), (value))
#define new_urlarg(ket, value) std::make_pair((key), (value));

tsp_request_t *tsp_request_new(void);
int tsp_request_delete(tsp_request_t *request);
int tsp_request_parse(const std::string &raw_request,
                      tsp_request_t *parse_result);
void tsp_request_handle(int sockfd, tsp_request_t *request);

#endif
