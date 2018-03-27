#include "logger.h"
#include "utility.h"

#include <cstdio>

TSPLogger *TSPLogger::logger = nullptr;
pthread_mutex_t TSPLogger::lock = PTHREAD_MUTEX_INITIALIZER;

void TSPLogger::log(const char *level, const char *fmt, va_list args) {
  char buff[1024];
  string now;
  TSPUtilTime::now(now);
  int siz = snprintf(buff, 1024, "[%s] %s : ", level, now.c_str());
  siz += vsnprintf(buff + siz, 1024 - siz, fmt, args);
}
