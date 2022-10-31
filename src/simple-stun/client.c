#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int descriptor = -1;

  struct sockaddr_in sin_server, sin;
  memset(&sin, 0, sizeof(sin));
  memset(&sin_server, 0, sizeof(sin_server));

  descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (descriptor < 0) {
    printf("Socket creation failed");
    return -1;
  }

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("0.0.0.0");
  sin.sin_port = htons(atoi(argv[1]));

  if (bind(descriptor, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    printf("Failed to bind");
    return -1;
  };

  sin_server.sin_family = AF_INET;
  sin_server.sin_addr.s_addr = inet_addr(argv[2]);
  sin_server.sin_port = htons(atoi(argv[3]));
  

  if (connect(descriptor, (const struct sockaddr *)&sin_server, sizeof(sin_server)) < 0) {
    printf("Failed to connect\n");
    close(descriptor);
    return -1;
  }

  printf("Connected");
  close(descriptor);

  return 0;
}
