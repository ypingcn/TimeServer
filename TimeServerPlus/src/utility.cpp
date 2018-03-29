#include "utility.h"

#include <iostream>

int TSPUtilTime::tostring(time_t t, string &res, const char *fmt) {
  struct tm *time_tm = localtime(&t);
  char buff[128];
  strftime(buff, sizeof(buff), fmt, time_tm);
  res = string(buff);
  return 0;
}

int TSPUtilTime::now(string &res) {
  time_t t = time(NULL);
  return tostring(t, res, "%F %X");
}

int TSPUtilFile::exist(const char *path) {
  int res = open(path, O_RDONLY | O_NOFOLLOW), dir = open(path, O_DIRECTORY);
  if (dir != -1)
    res = -1;
  close(dir);
  close(res);
  return res == -1 ? -1 : 0;
}

int TSPUtilFile::size(const char *path, off_t &size) {
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

void TSPUtilMemory::basic_free(void *p) { free(p); }

void *TSPUtilMemory::basic_malloc(size_t size) {
  void *res = malloc(size);
  if (res == NULL) {
    perror("tsp_util_malloc");
    exit(-1);
  }
  return res;
}

int TSPUtilIO::basic_write(int fd, char *buf, uint32_t cnt) {
  while (true) {
    int n = write(fd, buf, cnt);
    if (n >= 0)
      return n;
    int err = errno;
    switch (err) {
    case EINTR:
      break;
    case EAGAIN:
      return TSP_AGAIN;
    default:
      return TSP_ERROR;
    }
  }
}

int TSPUtilIO::basic_read(int fd, char *buf, uint32_t cnt) {
  while (true) {
    int n = read(fd, buf, cnt);
    if (n >= 0)
      return n;
    int err = errno;
    switch (err) {
    case EINTR:
      break;
    case EAGAIN:
      return TSP_AGAIN;
    default:
      return TSP_ERROR;
    }
  }
}
