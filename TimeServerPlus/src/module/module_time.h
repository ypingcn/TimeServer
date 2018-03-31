#ifndef TSP_MODULE_TIME_H
#define TSP_MODULE_TIME_H

#include "../type.h"

#include <string>
#include <unistd.h>
#include <vector>

void tsp_module_time_init(std::vector<tsp_module_t> &v);

void *tsp_module_time_localtime(void *sockfd, void *request);
void *tsp_module_time_file_ctime(void *sockfd, void *request);
void *tsp_module_time_file_atime(void *sockfd, void *request);
void *tsp_module_time_file_mtime(void *sockfd, void *request);

#endif
