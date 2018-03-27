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

int tsp_util_file_last_modify(const char *path, string &res) {}

void *tsp_util_malloc(size_t size) {
  void *res = malloc(size);
  if (res == NULL) {
    perror("tsp_util_malloc");
    exit(-1);
  }
  return res;
}

void tsp_util_free(void *p) { free(p); }

int TSPUtilTime::tostring(time_t t, string &res) {
  struct tm *time_tm = localtime(&t);
  char buff[128];
  strftime(buff, sizeof(buff), "%F %X", time_tm);
  res = string(buff);
}

int TSPUtilFile::exist(const char *path) {
  int res = open(path, O_RDONLY | O_NOFOLLOW), dir = open(path, O_DIRECTORY);
  if (dir != -1)
    res = -1;
  close(dir);
  close(res);
  return res == -1 ? -1 : 0;
}

int TSPUtilFile::size(const char *path, int &size) {
  struct stat tmp;
  int res = stat(path, &tmp);
  if (res == -1) {
    size = 0;
    return -1;
  }
  size = tmp.st_size;
  return 0;
}

int TSPUtilFile::last_modify(const char *path, string &res) {
  struct stat tmp;
  int state = stat(path, &tmp);
  if (state == -1) {
    res = "";
    return -1;
  }
  TSPUtilTime::tostring(tmp.st_mtime, res);
  return 0;
}

int TSPUtilFile::last_access(const char *path, string &res) {
  struct stat tmp;
  int state = stat(path, &tmp);
  if (state == -1) {
    res = "";
    return -1;
  }
  TSPUtilTime::tostring(tmp.st_atime, res);
  return 0;
}

int TSPUtilFile::last_change(const char *path, string &res) {
  struct stat tmp;
  int state = stat(path, &tmp);
  if (state == -1) {
    res = "";
    return -1;
  }
  TSPUtilTime::tostring(tmp.st_ctime, res);
  return 0;
}

void TSPUtilMemory::free(void *p) { free(p); }

void *TSPUtilMemory::malloc(size_t size) {
  void *res = malloc(size);
  if (res == NULL) {
    perror("tsp_util_malloc");
    exit(-1);
  }
  return res;
}
