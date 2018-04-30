#ifndef TIME_SERVER_PLUS
#define TIME_SERVER_PLUS

#include "contrib/config.h"
#include "contrib/logger.h"
#include "contrib/utility.h"
#include "core/core.h"
#include "core/request.h"
#include "include/type.h"
#include "modules/load-module.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

int32_t tsp_server_thread_num = 0;
pthread_mutex_t tsp_server_thread_num_mutex = PTHREAD_MUTEX_INITIALIZER;

int tsp_server_init(int port);
void tsp_server_event_circles(int server_fd);
void *tsp_server_thread_function(void *param);

int tsp_server_thread_num_add();
int tsp_server_thread_num_del();
int32_t tsp_server_thread_num_get();

#endif
