#include "utility.h"

#include <iostream>

int tsp_util_file_exist(const char *path) {
  int res = open(path, O_RDONLY | O_NOFOLLOW), dir = open(path, O_DIRECTORY);
  if (dir != -1)
    res = -1;
  close(dir);
  close(res);
  return res == -1 ? -1 : 0;
}

off_t tsp_util_file_size(const char *path) {
  struct stat tmp;
  int res = stat(path, &tmp);
  if (res == -1) {
    perror("tsp_util_file_size");
    exit(-1);
  }
  return tmp.st_size;
}

int tsp_util_file_last_modify(const char *path, std::string &res) {}

void *tsp_util_malloc(size_t size) {
  void *res = malloc(size);
  if (res == NULL) {
    perror("tsp_util_malloc");
    exit(-1);
  }
  return res;
}

void tsp_util_free(void *p) { free(p); }
