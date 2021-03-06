#include "TimeServerPlus.h"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <config_path>" << endl;
    exit(-1);
  }
  if (TSPUtilFile::exist(argv[1]) == -1) {
    TSPLogger::instance()->error("config file not exist [%s]", strerror(errno));
    exit(-1);
  }
  if (TSPConfig::instance()->parse(argv[1]) == -1) {
    TSPLogger::instance()->error("config file parse error [%s]",
                                 strerror(errno));
    exit(-1);
  }

  if (TSPConfig::instance()->exist("DAEMON", "1") == 1) {
    if (daemon(1, 0) < 0) {
      std::cerr << "DAEMON ERROR" << '\n';
    }
  }

  // signal(SIGINT, SIG_IGN);
  // signal(SIGHUP, SIG_IGN);
  // signal(SIGQUIT, SIG_IGN);
  // signal(SIGPIPE, SIG_IGN);
  // signal(SIGTTOU, SIG_IGN);
  // signal(SIGTTIN, SIG_IGN);

  string port;
  if (TSPConfig::instance()->get("PORT", port) == -1 || port == "")
    port = "8600";
  cout << "TSP Running .. port :" << port << endl;

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
        e.data.fd = ev[i].data.fd;
        tsp_epoll_ctl(epollfd, EPOLL_CTL_DEL, ev[i].data.fd, &e);

        if (TSPConfig::instance()->exist("IPRECORD", "1") == 1) {
          struct sockaddr_in in_addr;
          socklen_t len = sizeof(in_addr);
          bzero(&in_addr, len);
          if (getpeername(ev[i].data.fd, (sockaddr *)&in_addr, &len) != -1) {
            TSPLogger::instance()->info("visitor's IP is %s [IPRECORD]",
                                        inet_ntoa(in_addr.sin_addr));
          }
        }

        pthread_t tid;
        pthread_create(&tid, &pthread_attr_detach, &tsp_server_thread_function,
                       (void *)(intptr_t)ev[i].data.fd);
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

  TSPRequest req(client_socket_fd);

BEGIN:
  int32_t nread = 0, n = 0;
  int nfds = 0;

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
      TSPLogger::instance()->error(
          "thread function error : read socket timeout [%s]", strerror(errno));
      goto OUT;
    } else {
      TSPLogger::instance()->error(
          "thread function error : read other error [%s]", strerror(errno));
      break;
    }
  }

  if (nread != 0) {
    string raw_request(buff, buff + nread);

    if (req.parse_from_string(raw_request) == -1) {
      TSPLogger::instance()->error(
          "thread function error : request parse fail [%s]", strerror(errno));
      tsp_response_bad_request(client_socket_fd);
      tsp_response_headers_end(client_socket_fd);
      goto BEGIN;
    } else {
      tsp_request_handle(req);
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
  close(thread_epollfd);
  TSPUtilMemory::basic_free(buff);
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
