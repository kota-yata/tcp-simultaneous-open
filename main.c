#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  char* message = "Hello :)";
  char* buffer = malloc(BUF_SIZE);
  int fd_in;
  int fd_out;
  int fd_misc;

  int m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket < 0) {
    printf("Socket generation failed\n");
    return -1;
  }

  struct sockaddr_in my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = inet_addr(argv[1]);
  my_addr.sin_port = htons(atoi(argv[2]));

  struct sockaddr_in peer_addr;
  memset(&peer_addr, 0, sizeof(peer_addr));
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_addr.s_addr = inet_addr(argv[3]);
  peer_addr.sin_port = htons(atoi(argv[4]));

  int bind_res = bind(fd_in, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if (bind_res < 0) {
    printf("Failed to bind");
    close(fd_in);
    return -1;
  }

  // Connection Attempt
  int connect_res = connect(fd_out, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if (connect_res < 0) {
    printf("Connection attempt failed\n");
    return - 1;
  }

  send(0, message, strlen(message), 0);
  ssize_t received_val = recv(fd_misc, buffer, BUF_SIZE, 0);
  printf("%s\n", buffer);

  free(buffer);

  close(fd_in);

  return 0;
}
