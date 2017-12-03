#ifndef TSP_MODULE_BASIC_H
#define TSP_MODULE_BASIC_H

#include "../type.h"
#include "../config.h"
#include "../response.h"

#include <unistd.h>
#include <sys/sendfile.h>
#include <regex>
#include <string>

using std::string;
using std::regex;

void tsp_module_basic_init(std::vector<tsp_module_t>& v);

void * tsp_module_basic_get(void * sockfd,void * request);
void * tsp_module_basic_head(void * sockfd,void * request);
void * tsp_module_basic_options(void * sockfd,void * request);

void * tsp_module_basic_post(void * sockfd,void * request);
void * tsp_module_basic_put(void * sockfd,void * request);
void * tsp_module_basic_delete(void * sockfd,void * request);

#endif
