#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int descriptor = -1;

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));

  descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (descriptor < 0) {
    printf("Socket creation failed");
    return -1;
  }

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(argv[1]);
  sin.sin_port = htons(atoi(argv[2]));

  if (connect(descriptor, (const struct sockaddr *)&sin, sizeof(sin)) < 0) {
    printf("Failed to connect");
    close(descriptor);
    return -1;
  }

  printf("Connected");
  close(descriptor);

  return 0;
}
