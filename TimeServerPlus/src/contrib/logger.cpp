#include "logger.h"
#include "config.h"
#include "utility.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>

TSPLogger *TSPLogger::logger = nullptr;
pthread_mutex_t TSPLogger::lock = PTHREAD_MUTEX_INITIALIZER;

void TSPLogger::log(const char *level, const char *fmt, va_list args) {
  char buff[1024];
  string now;
  TSPUtilTime::now(now);
  pthread_t pid = pthread_self();
  int siz = snprintf(buff, 1024, "[%s][pid:%u] %s : ", level, (unsigned int)pid,
                     now.c_str());
  siz += vsnprintf(buff + siz, 1024 - siz, fmt, args);
  if (siz == 1024) {
    buff[1023] = '\n';
  } else {
    buff[siz] = '\n';
    siz++;
    buff[siz] = '\0';
  }
  string log_file_name;
  if (TSPConfig::instance()->get("LOG", log_file_name) != -1) {
    int fd = open(log_file_name.data(), O_CREAT | O_RDWR | O_APPEND,
                  S_IRUSR | S_IWUSR);
    TSPUtilIO::basic_write(fd, buff, siz);
    close(fd);
  }
}
