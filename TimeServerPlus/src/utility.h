#ifndef TSP_UTILITY
#define TSP_UTILITY

#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

int tsp_util_file_exist(const char *path);
off_t tsp_util_file_size(const char *path);
int tsp_util_file_last_modify(const char *path, std::string &res);
int tsp_util_gmt(std::string &res);
void *tsp_util_malloc(size_t size);
void tsp_util_free(void *p);

#endif
