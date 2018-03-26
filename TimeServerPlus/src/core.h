#ifndef TSP_CORE
#define TSP_CORE

#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>

int tsp_socket(int domain, int type, int protocol);
int tsp_socket_nonblocking(int sockfd);
int tsp_socket_reuseaddr(int sockfd);
void tsp_socket_nodelay(int sockfd);
void tsp_socket_set_timeout(int sockfd, int sec, int usec);
void tsp_socket_bind(int sockfd, const struct sockaddr *addr, socklen_t len);
void tsp_socket_listen(int sockfd, int backlog);
int tsp_socket_accept(int sockfd, struct sockaddr *addr, socklen_t *len);

int tsp_epoll_create(int size);
void tsp_epoll_ctl(int epollfd, int op, int fd, struct epoll_event *event);
int tsp_epoll_wait(int epollfd, struct epoll_event *events, int maxevents,
                   int timeout);

#endif
