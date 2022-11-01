#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_MAX 200

int main(int argc, char *argv[]) {
  int descriptor = -1;
  unsigned char buffer[BUF_MAX];
  unsigned char binding_request[20];
  memset(&binding_request, 0, sizeof(binding_request));

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
    printf("Failed to bind\n");
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

  printf("Connected\n");

  *(short *)(&binding_request[0]) = htons(0x0001); // Message Type (Binding Request this time)
  *(int *)(&binding_request[4]) = htonl(0x2112A442); // Magic Cookie (Fixed value to distinguish STUN traffic from other protocols)
  *(int *)(&binding_request[8]) = htonl(0x471B519F); // Transaction ID (Random value to pair up a request and corresponding response)

  printf("Sending Binding Request...");
  if (send(descriptor, &binding_request, sizeof(binding_request), 0) < 0) {
    printf("Failed\n");
    close(descriptor);
    return -1;
  }

  printf("Sent\nReceiving Binding Response...");
  if (recv(descriptor, &buffer, BUF_MAX, 0) < 0) {
    printf("Failed\n");
    close(descriptor);
    return  -1;
  }

  // 0x0101 at the first two bytes means this is Binding Response
  // that being said the response is successfully received
  if (*(short *)(&buffer[0]) == htons(0x0101)) {
    printf("Received\n");
    int i = 20; // Data section starts after the header, which is 20 bytes
    short attribute_type;
    short attribute_length;
    unsigned short port;
    // Continuously read attributes in the data section
    while(i < sizeof(buffer)) {
      attribute_type = htons(*(short *)(&buffer[i]));
      attribute_length = htons(*(short *)(&buffer[i + 2]));
      // If the attribute is XOR_MAPPED_ADDRESS, parse it
      if (attribute_type == 0x0020) {
        port = ntohs(*(short *)(&buffer[i + 6]));
        printf("%d\n", port);
        port ^= 0x2112;
        printf("%d.%d.%d.%d:%d\n", buffer[i + 8] ^ 0x21, buffer[i + 9] ^ 0x12, buffer[i + 10] ^ 0xA4, buffer[i + 11] ^ 0x42, port);
        break;
      }
      i += 4 + attribute_length;
    }
  }
  close(descriptor);

  return 0;
}
