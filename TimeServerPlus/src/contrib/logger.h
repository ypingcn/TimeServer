#ifndef TSP_LOGGER
#define TSP_LOGGER

#include <cstdarg>
#include <pthread.h>

#define level_log(level)                                                       \
  va_list args;                                                                \
  va_start(args, fmt);                                                         \
  log(level, fmt, args);                                                       \
  va_end(args);

enum class TSPLogLevel { INFO, ALERT, WARN, ERROR };

class TSPLogger {

public:
  TSPLogger(const TSPLogger &) = delete;
  TSPLogger &operator=(const TSPLogger &) = delete;

  void info(const char *fmt, ...) { level_log("INFO"); }
  void alert(const char *fmt, ...) { level_log("ALERT"); }
  void warn(const char *fmt, ...) { level_log("WARN"); }
  void error(const char *fmt, ...) { level_log("ERROR"); }

  static TSPLogger *instance() {
    if (logger == nullptr) {
      pthread_mutex_lock(&lock);
      if (logger == nullptr)
        logger = new TSPLogger();
      pthread_mutex_unlock(&lock);
    }
    return logger;
  }

private:
  TSPLogger() = default;
  ~TSPLogger() = default;
  void log(const char *level, const char *fmt, va_list args);

  static TSPLogger *logger;
  static pthread_mutex_t lock;
};

#endif
