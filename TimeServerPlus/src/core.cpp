#include "core.h"

int tsp_socket(int domain,int type,int protocol)
{
  int fd = socket(domain, type, protocol);
  if(fd == -1)
  {
    perror("tsp_socket:");
    exit(-1);
  }
  return fd;
}

int tsp_socket_nonblocking(int sockfd)
{
  int f = fcntl(sockfd,F_GETFL,0);
  if(f < 0)
  {
    perror("tsp_socket_nonblocking,GETFL");
    exit(-1);
  }
  f |= O_NONBLOCK;
  int res = fcntl(sockfd, F_SETFL, f);
  if(res < 0)
  {
    perror("tsp_socket_nonblocking,SETFL");
    exit(-1);
  }
}

int tsp_socket_reuseaddr(int sockfd)
{
  int reuse_on = -1;
  int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_on, sizeof(reuse_on));
  if(res < 0)
  {
    perror("tsp_socket_reuseaddr");
    exit(-1);
  }
}

void tsp_socket_nodelay(int sockfd)
{
  int nodelay = 1;
  int res = setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY, &nodelay, sizeof(nodelay));
  if(res == -1)
  {
    perror("tsp_socket_nodelay:");
    exit(-1);
  }
}

void tsp_socket_set_timeout(int sockfd,int sec,int usec)
{
  struct timeval val= {sec,usec};
  int res = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &val, sizeof(val));
  if(res == -1)
  {
    perror("tsp_socket_set_timeout:");
    exit(-1);
  }
}
void tsp_socket_bind(int sockfd,const struct sockaddr * addr,socklen_t len)
{
  if( bind(sockfd,addr,len) == -1)
  {
    perror("tsp_socket_bind");
    exit(-1);
  }
}

void tsp_socket_listen(int sockfd,int backlog)
{
  if(listen(sockfd,backlog) == -1)
  {
    perror("tsp_socket_listen");
    exit(-1);
  }
}

int tsp_socket_accept(int sockfd,struct sockaddr * addr,socklen_t * len)
{
  int new_sockfd = 0;
  while(true)
  {
    new_sockfd = accept(sockfd, addr, len);
    if(new_sockfd > 0)
      return new_sockfd;
    if(new_sockfd == -1)
    {
      if(errno != EAGAIN && errno != EINTR
        && errno != EPROTO && errno != ECONNABORTED)
      {
        perror("tsp_socket_accept");
        exit(-1);
      }
    }
  }
}

int tsp_epoll_create(int size)
{
  int epollfd = epoll_create(size);
  if(epollfd == -1)
  {
    perror("tsp_epoll_create");
    exit(-1);
  }
  return epollfd;
}

void tsp_epoll_ctl(int epollfd,int op,int fd,struct epoll_event * event)
{
  if(epoll_ctl(epollfd,op,fd,event) == -1)
  {
    perror("tsp_epoll_ctl");
    exit(-1);
  }
}

int tsp_epoll_wait(int epollfd,struct epoll_event * events,int maxevents,int timeout)
{
  int num = epoll_wait(epollfd, events, maxevents, timeout);
  while( num == -1 )
  {
    if( errno != EINTR)
    {
      perror("tsp_epoll_wait");
      exit(-1);
    }
    num = epoll_wait(epollfd, events, maxevents, timeout);
  }
  return num;
}
