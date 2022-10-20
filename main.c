#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Insufficient number of arguments\n");
    exit(EXIT_FAILURE);
  }

  char* message = "Hello :)";
  char buffer[BUF_SIZE] = { 0 };

  int m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket < 0) {
    printf("Socket generation failed\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in peer_addr;
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons(argv[2]);

  int pton_res = inet_pton(AF_INET, argv[1], &peer_addr.sin_addr);
  if (pton_res <= 0) {
    printf("Conversion from string to binary failed\n");
    exit(EXIT_FAILURE);
  }

  // Connection Attempt
  int fd = connect(0, (struct sockaddr*) &peer_addr, sizeof(peer_addr));
  if (fd < 0) {
    printf("Connection attempt failed\n");
    exit(EXIT_FAILURE);
  }

  send(0, message, strlen(message), 0);
  ssize_t received_val = read(0, buffer, BUF_SIZE);
  printf("%s\n", buffer);

  close(fd);

  return EXIT_SUCCESS;
}
