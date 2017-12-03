#ifndef TSP_MODULE_TIME_H
#define TSP_MODULE_TIME_H

#include "../type.h"

#include <string>
#include <vector>
#include <unistd.h>

void tsp_module_time_init(std::vector<tsp_module_t>& v);

void * tsp_module_time_localtime(void * sockfd,void * request);

#endif
