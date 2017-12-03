#ifndef TSP_UTILITY
#define TSP_UTILITY

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int tsp_util_file_exist(const char * path);
off_t tsp_util_file_size(const char * path);
int tsp_util_file_last_modify(const char * path,std::string& res);
int tsp_util_gmt(std::string& res);
void * tsp_util_malloc(size_t size);
void tsp_util_free(void * p);
#endif
