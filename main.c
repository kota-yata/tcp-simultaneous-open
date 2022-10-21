#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

int construct_sockaddr(struct sockaddr_in* addr_struct, char* addr, char* port) {
  memset(addr_struct, 0, sizeof(*addr_struct));
  addr_struct->sin_family = AF_INET;
  addr_struct->sin_addr.s_addr = inet_addr(addr);
  addr_struct->sin_port = htons(atoi(port));
  return 0;
}

int main(int argc, char *argv[]) {
  char* message = "Hello :)";
  int fd_in = -1;
  int fd_out = -1;
  int fd_misc = -1;

  fd_in = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_in < 0) {
    printf("Socket generation failed\n");
    return -1;
  }

  struct sockaddr_in my_addr;
  construct_sockaddr(&my_addr, argv[1], argv[2]);
  struct sockaddr_in peer_addr;
  construct_sockaddr(&my_addr, argv[3], argv[4]);

  int bind_res = bind(fd_in, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if (bind_res < 0) {
    printf("Failed to bind\n");
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
  char* buffer = malloc(BUF_SIZE);
  ssize_t received_val = recv(fd_misc, buffer, BUF_SIZE, 0);
  printf("%s\n", buffer);
  free(buffer);
  close(fd_in);

  return 0;
}
