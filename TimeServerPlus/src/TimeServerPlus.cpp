#include "TimeServerPlus.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <config_path>" << endl;
    exit(-1);
  }
  if (TSPUtilFile::exist(argv[1]) == -1) {
    perror("tsp_util_file_exist:");
    exit(-1);
  }
  if (tsp_config_parse(argv[1]) == -1) {
    perror("tsp_config_parse:");
    exit(-1);
  }

  string port;
  if (tsp_config_get("PORT", port) == -1 || port == "")
    port = "8600";
  cout << "TSP Running .. port :" << port << endl;

  tsp_module_init();

  int server_socket_fd = tsp_server_init(stoi(port));
  tsp_server_event_circles(server_socket_fd);
  return 0;
}

int tsp_server_init(int port) {
  int server_socket_fd = tsp_socket(AF_INET, SOCK_STREAM, 0);
  tsp_socket_nonblocking(server_socket_fd);
  tsp_socket_reuseaddr(server_socket_fd);

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  tsp_socket_bind(server_socket_fd, (struct sockaddr *)&server_addr,
                  sizeof(server_addr));
  tsp_socket_listen(server_socket_fd, MAX_BACKLOG);

  return server_socket_fd;
}

void tsp_server_event_circles(int server_fd) {
  int epollfd = tsp_epoll_create(MAX_EVENT_SIZE);

  struct epoll_event e;
  e.events = EPOLLIN;
  e.data.fd = server_fd;

  tsp_epoll_ctl(epollfd, EPOLL_CTL_ADD, server_fd, &e);

  pthread_attr_t pthread_attr_detach;
  pthread_attr_init(&pthread_attr_detach);
  pthread_attr_setdetachstate(&pthread_attr_detach, PTHREAD_CREATE_DETACHED);

  struct epoll_event ev[MAX_EVENT_SIZE];
  int client_socket_fd;

  while (true) {

    int num = tsp_epoll_wait(epollfd, ev, MAX_EVENT_SIZE, -1);

    if (num == -1 && errno == EINTR)
      continue;

    struct sockaddr_in client_addr;
    socklen_t addrlen;

    for (int i = 0; i != num; i++) {
      if (ev[i].data.fd == server_fd) {
        client_socket_fd = tsp_socket_accept(
            server_fd, (struct sockaddr *)&client_addr, &addrlen);
        tsp_socket_nonblocking(client_socket_fd);
        e.events = EPOLLIN | EPOLLET;
        e.data.fd = client_socket_fd;
        tsp_epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket_fd, &e);

      } else {
        e.data.fd = client_socket_fd;
        tsp_epoll_ctl(epollfd, EPOLL_CTL_DEL, client_socket_fd, &e);

        pthread_t tid;
        pthread_create(&tid, &pthread_attr_detach, &tsp_server_thread_function,
                       (void *)(intptr_t)client_socket_fd);
      }
    }
  }

  pthread_attr_destroy(&pthread_attr_detach);
  close(server_fd);
}

void *tsp_server_thread_function(void *param) {
  tsp_server_thread_num_add();
  pthread_t thread_id = pthread_self();
  cout << "Thread " << (unsigned int)thread_id << " Running..." << endl;

  int client_socket_fd = (intptr_t)param;

  struct epoll_event e, ev[2];
  e.events = EPOLLIN | EPOLLET;
  e.data.fd = client_socket_fd;

  int thread_epollfd = tsp_epoll_create(2);
  tsp_epoll_ctl(thread_epollfd, EPOLL_CTL_ADD, client_socket_fd, &e);

  tsp_socket_nodelay(client_socket_fd);
  tsp_socket_set_timeout(client_socket_fd, 60, 0);

  char *buff = (char *)TSPUtilMemory::basic_malloc(BUFF_SIZE);
  bzero(buff, BUFF_SIZE);

BEGIN:
  int32_t nread = 0, n = 0;
  int nfds = 0;
  tsp_request_t *thread_request = tsp_request_new();

  for (;;) {
    n = read(client_socket_fd, buff + nread, BUFF_SIZE - 1);
    if (n > 0)
      nread += n;
    else if (n == 0)
      break;
    else if (n == -1 && errno == EINTR)
      continue;
    else if (n == -1 && errno == EAGAIN)
      break;
    else if (n == -1 && errno == EWOULDBLOCK) {
      perror("tsp_server_thread_function: read socket timeout");
      goto OUT;
    } else {
      perror("tsp_server_thread_function: read other error");
      TSPUtilMemory::basic_free(buff);
      break;
    }
  }

  if (nread != 0) {
    string raw_request(buff, buff + nread);

    if (tsp_request_parse(raw_request, thread_request) == -1) {
      perror("tsp_server_thread_function: request parse fail");
      tsp_response_bad_request(client_socket_fd);
      tsp_response_headers_end(client_socket_fd);
      tsp_request_delete(thread_request);
      goto BEGIN;
    } else {
      tsp_request_handle(client_socket_fd, thread_request);
    }

    nfds = tsp_epoll_wait(thread_epollfd, ev, 2, 1000);
    if (0 == nfds) // timeout
      goto OUT;
    for (int i = 0; i < nfds; i++) {
      if (ev[i].data.fd == client_socket_fd)
        goto BEGIN;
      else
        goto OUT;
    }
  }

OUT:
  close(client_socket_fd);
  tsp_request_delete(thread_request);
  tsp_server_thread_num_del();
  printf("Thread %u exit\n", (unsigned int)thread_id);
}

int tsp_server_thread_num_add() {
  pthread_mutex_lock(&tsp_server_thread_num_mutex);
  tsp_server_thread_num++;
  pthread_mutex_unlock(&tsp_server_thread_num_mutex);
  return 0;
}
int tsp_server_thread_num_del() {
  int res = 0;
  pthread_mutex_lock(&tsp_server_thread_num_mutex);
  if (tsp_server_thread_num < 0)
    res = -1;
  else
    tsp_server_thread_num--;
  pthread_mutex_unlock(&tsp_server_thread_num_mutex);
  return res;
}
int32_t tsp_server_thread_num_get() {
  int32_t res;
  pthread_mutex_lock(&tsp_server_thread_num_mutex);
  res = tsp_server_thread_num;
  pthread_mutex_unlock(&tsp_server_thread_num_mutex);
  return res;
}
