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
  int fd_out = -1;
  int fd_in = -1;

  fd_out = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_out < 0) {
    printf("Socket generation failed\n");
    return -1;
  }

  struct sockaddr_in my_addr;
  construct_sockaddr(&my_addr, argv[1], argv[2]);
  struct sockaddr_in peer_addr;
  construct_sockaddr(&peer_addr, argv[3], argv[4]);

  if (bind(fd_out, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0) {
    printf("Failed to bind %s\n", argv[1]);
    close(fd_out);
    return -1;
  }

  int wait_for = get_remaining_sec();
  printf("Waiting for %ds\n", wait_for);
  sleep(wait_for);

  // Connection Attempt
  if (connect(fd_out, (struct sockaddr*) &peer_addr, sizeof(peer_addr)) < 0) {
    printf("Connection attempt failed\n");
    return - 1;
  }
  printf("Connected!");

  // write(fd_out, message, sizeof(message));
  if (send(fd_out, message, sizeof(message), 0) < 0) {
    printf("Failed to send message");
    return -1;
  }
  char* buffer = malloc(BUF_SIZE);
  if (read(fd_in, buffer, BUF_SIZE) < 0) {
    printf("Failed to read message");
    return -1;
  }
  printf("Received: %s \n", buffer);
  free(buffer);
  close(fd_out);
  close(fd_in);

  return 0;
}
