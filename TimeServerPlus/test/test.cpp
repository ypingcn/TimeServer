#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " <IP ADDRESS>" << endl;
    return -1;
  }

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    cout << "socket error" << endl;

  struct sockaddr_in server;
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(8668);

  if (inet_pton(sockfd, argv[1], &server.sin_addr))
    cout << "something wrong in IP address" << endl;

  if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    cout << "connect error" << endl;
  } else {
    cout << "send begin" << endl;
    char header[] = "GET /time HTTP/1.1\r\n\r\n";
    write(sockfd, &header, sizeof(header));
    cout << "send end" << endl;
    cout << "read begin" << endl;
    int n = 0, nread = 0;
    char result[204800];
    while ((n = read(sockfd, result + nread, sizeof(result))) > 0) {
      nread += n;
    }
    result[nread] = '\0';
    cout << "read end" << endl;
    cout << "result: ----" << endl;
    cout << result << endl;
  }

  return 0;
}
