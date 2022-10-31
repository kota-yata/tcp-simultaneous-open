#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
// #include <pcap/pcap.h>

#define SERVER_ADDR "172.16.5.4"
#define SERVER_PORT 55555

int main(void) {
  int socket_descriptor = -1;
  int misc_descriptor = 01;

  struct sockaddr_in sin, sin_client;
  memset(&sin, 0, sizeof(sin));
  memset(&sin_client, 0, sizeof(sin_client));

  // Create a TCP server socket
  socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_descriptor < 0) {
    printf("Socket creation failed\n");
    return -1;
  }

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(SERVER_PORT);

  // Bind the server socket
  if (bind(socket_descriptor, (const struct sockaddr *)&sin, sizeof(sin)) < 0) {
    printf("Failed to bind\n");
    return -1;
  }

  // Start listening to up to 10 connection attempts
  if (listen(socket_descriptor, 10) < 0) {
    printf("Failed to listen\n");
    close(socket_descriptor);
    return -1;
  }

  socklen_t sin_client_len = sizeof(sin_client);

  while(1) {
    misc_descriptor = accept(socket_descriptor, (struct sockaddr *)&sin_client, &sin_client_len);
    if (misc_descriptor < 0) {
      printf("Failed to accept, waiting again...\n");
      continue;
    }
    // Receive header data
    printf("Connected");
    close(misc_descriptor);
  }
  return 0;
}
