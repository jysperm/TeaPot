#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <string>

#define BUF_SIZE 4096
#define PORT 3000
#define SERVER_STRING "Server: TeaPot Alpha 0.1\r\n"

void ErrorHandler(int err, std::string method) {
  perror(method.c_str());
}

void RequestHandler(int client) {
  char buffer[BUF_SIZE];
  memset(buffer, 0, BUF_SIZE);

  strcpy(buffer, "HTTP/1.1 200 OK\r\n");
  send(client, buffer, strlen(buffer), 0);
  strcpy(buffer, SERVER_STRING);
  send(client, buffer, strlen(buffer), 0);
  sprintf(buffer, "Content-Type: text/plain\r\n");
  send(client, buffer, strlen(buffer), 0);
  strcpy(buffer, "\r\n");
  send(client, buffer, strlen(buffer), 0);

  sprintf(buffer, "Hello TeaPot!");
  send(client, buffer, strlen(buffer), 0);

  close(client);
}

int main(int argc, char * argv[]) {
  int server_sock = -1, client_sock = -1, ret = -1;
  socklen_t client_len = 0;
  struct sockaddr_in server_addr, client_addr;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock == -1)
    ErrorHandler(server_sock, "socket");
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  ret = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret == -1)
    ErrorHandler(ret, "bind");
  ret = listen(server_sock, 5);
  if (ret == -1)
    ErrorHandler(ret, "listen");
  while (1) {
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock == -1)
      ErrorHandler(client_sock, "accept");
    RequestHandler(client_sock);

  }
  close(server_sock);
  return 0;
}
