#include "module_time.h"
#include "../response.h"


#include <pthread.h>
#include <iostream>
using std::cout；
using std::endl;

void tsp_module_time_init(std::vector<tsp_module_t>& v)
{
  tsp_module_t a = {
     {
       std::regex("^/time.*"),
       std::string("GET"),
       std::string("HTTP/1.1")
     },
     tsp_module_time_localtime,
     100
  };
  v.push_back(a);
}

void * tsp_module_time_localtime(void * sockfd,void * request)
{
  int client_sockfd = (intptr_t)sockfd;

  tsp_response_ok(client_sockfd);
  tsp_response_headers_end(client_sockfd);

  char body[1024];
  struct tm * now;
  time_t timer =  time(NULL);
  now = localtime(&timer);
  strftime(body, sizeof(body), "<p>Now: %F %X<p>\n",now );

  write(client_sockfd,body,strlen(body));
}
