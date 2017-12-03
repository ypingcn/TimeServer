#ifndef TIME_SERVER_PLUS
#define TIME_SERVER_PLUS

#include "config.h"
#include "core.h"
#include "request.h"
#include "utility.h"

#include "module/load-module.h"

#include <cstring>
#include <netinet/in.h>
#include <pthread.h>
#include <string>

#define MAX_BACKLOG 200
#define MAX_EVENT_SIZE 1024
#define KILO 1024
#define BUFF_SIZE 1024*KILO

int32_t tsp_server_thread_num = 0;
pthread_mutex_t tsp_server_thread_num_mutex = PTHREAD_MUTEX_INITIALIZER;

int tsp_server_init(int port);
void tsp_server_event_circles(int server_fd);
void * tsp_server_thread_function(void* param);

int tsp_server_thread_num_add();
int tsp_server_thread_num_del();
int32_t tsp_server_thread_num_get();

#endif
