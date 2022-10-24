#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 1024

int construct_sockaddr(struct sockaddr_in* addr_struct, char* addr, char* port) {
  memset(addr_struct, 0, sizeof(*addr_struct));
  addr_struct->sin_family = AF_INET;
  addr_struct->sin_addr.s_addr = inet_addr(addr);
  addr_struct->sin_port = htons(atoi(port));
  return 0;
}

int get_remaining_sec() {
  time_t t = time(NULL);
  struct tm tm;
  gmtime_r(&t, &tm);
  return 60 - tm.tm_sec;
}

int main(int argc, char *argv[]) {
  char* message = "Hello :)";
  int fd_in = -1;

  fd_in = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_in < 0) {
    printf("Socket generation failed\n");
    return -1;
  }

  struct sockaddr_in my_addr;
  construct_sockaddr(&my_addr, argv[1], argv[2]);
  struct sockaddr_in peer_addr;
  construct_sockaddr(&peer_addr, argv[3], argv[4]);

  int bind_res = bind(fd_in, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if (bind_res < 0) {
    printf("Failed to bind %s\n", argv[1]);
    close(fd_in);
    return -1;
  }

  int wait_for = get_remaining_sec();
  printf("Waiting for %ds\n", wait_for);
  sleep(wait_for);

  // Connection Attempt
  int connect_res = connect(fd_in, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if (connect_res < 0) {
    printf("Connection attempt failed\n");
    return - 1;
  }

  write(fd_in, message, sizeof(message));
  char* buffer = malloc(BUF_SIZE);
  int read_res = read(fd_in, buffer, BUF_SIZE);
  fprintf(stderr, "Received \"%.*s\" from socket\n", read_res, buffer);
  free(buffer);
  close(fd_in);

  return 0;
}
