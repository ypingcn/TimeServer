#ifndef TSP_UTILITY
#define TSP_UTILITY

#include "../include/type.h"

#include <cstdarg>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

enum ResultCode { TSP_OK = 0, TSP_ERROR = -1, TSP_AGAIN = -2 };

class TSPUtilTime {
public:
  static int tostring(time_t t, string &res, const char *fmt = "%F %X");
  static int now(string &res);
};

class TSPUtilFile {
public:
  static int exist(const char *path);
  static int is_directory(const char *path);
  static int size(const char *path, off_t &size);
  static int last_modify(const char *path, string &res);
  static int last_access(const char *path, string &res);
  static int last_change(const char *path, string &res);
};

class TSPUtilMemory {
public:
  static void basic_free(void *p);
  static void *basic_malloc(size_t size);
};

class TSPUtilIO {
public:
  static int basic_write(int fd, char *buf, uint32_t cnt);
  static int basic_read(int fd, char *buf, uint32_t cnt);
};

#endif
