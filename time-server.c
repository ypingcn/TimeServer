#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define SERVER_STRING "Server: time-server/0.1.170924(c)\r\n"

void error_exit(const char *s) {
  perror(s);
  exit(1);
}

int init(uint16_t port) {
  int httpd = socket(AF_INET, SOCK_STREAM, 0);
  if (httpd == -1)
    error_exit("socket init failed in init\n");

  struct sockaddr_in name;
  memset(&name, 0, sizeof(name));
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  int reuse_on = 1;
  if (setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &reuse_on, sizeof(reuse_on)) <
      0)
    error_exit("setsocketopt failed in init\n");
  if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
    error_exit("bind failed in init\n");

  if (listen(httpd, 1000) < 0)
    error_exit("listen failed\n");

  return httpd;
}

int get_socket_line(int sockfd, char *buf, int size) {
  int i = 0;
  char in = '\0';
  int len;
  while (i < size - 1 && in != '\n') {
    len = recv(sockfd, &in, 1, 0);
    if (len > 0) {
      if (in == '\r') {
        len = recv(sockfd, &in, 1, MSG_PEEK);
        if (len > 0 && in == '\n')
          recv(sockfd, &in, 1, 0);
        else
          in = '\n';
      }
      buf[i] = in;
      i++;
    } else
      in = '\n';
  }
  buf[i] = '\0';
  return i;
}

void service(int sockfd) {
  char buf[1024];

  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, SERVER_STRING);
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, "Content-Type: text/html\r\n");
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, "\r\n");
  send(sockfd, buf, strlen(buf), 0);

  struct tm *now;
  time_t timer = time(NULL);
  now = localtime(&timer);
  strftime(buf, sizeof(buf), "<p> Now: %F %X<p>", now);
  send(sockfd, buf, strlen(buf), 0);
}

void refuse_to_service(int sockfd) {
  char buf[1024];

  sprintf(buf, "HTTP/1.0 403 Forbidden\r\n");
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, SERVER_STRING);
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, "Content-Type: text/html\r\n");
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, "\r\n");
  send(sockfd, buf, strlen(buf), 0);
  sprintf(buf, "<p> Refuse </p>");
  send(sockfd, buf, strlen(buf), 0);
}

void accept_request(void *arg) {
  int client_socket = (intptr_t)arg;
  printf("%d\n", client_socket);
  char headers[512];
  char method[512];
  get_socket_line(client_socket, headers, sizeof(headers));

  int i = 0;
  while (!isspace(headers[i]) && i < sizeof(method) - 1) {
    method[i] = headers[i];
    i++;
  }
  method[i] = '\0';

  char buf[255];
  int len = get_socket_line(client_socket, buf, sizeof(buf));
  while (len > 0 && strcmp(buf, "\n")) {
    printf("(%d)##: %s", client_socket, buf);
    len = get_socket_line(client_socket, buf, sizeof(buf));
  }

  if (strcasecmp(method, "GET") == 0)
    service(client_socket);
  else
    refuse_to_service(client_socket);

  close(client_socket);
}

int main(int argc, char const *argv[]) {
  uint16_t port = 8600;
  int server_socket = -1;

  server_socket = init(port);
  printf("running ...\n");

  int client_socket = -1;
  struct sockaddr_in client_name;
  socklen_t client_name_len = sizeof(client_name);
  pthread_t new_pthread;

  while (1) {
    client_socket = accept(server_socket, (struct sockaddr *)&client_name,
                           &client_name_len);
    if (client_socket == -1)
      error_exit("accept failed\n");
    if (pthread_create(&new_pthread, NULL, (void *)accept_request,
                       (void *)(intptr_t)client_socket) < 0)
      error_exit("pthread_create failed\n");
  }

  close(server_socket);
  return 0;
}
