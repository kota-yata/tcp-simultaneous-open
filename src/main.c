#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

int get_remaining_msec();

int main(int argc, char *argv[]) {
  char* message = "Hello :)";
  int misc_descriptor = -1;
  int my_descriptor = -1;
  int connect_res = -1;

  my_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (my_descriptor < 0) {
    printf("Socket generation failed\n");
    return -1;
  }

  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = inet_addr(argv[1]);
  my_addr.sin_port = htons((unsigned short) atoi(argv[2]));

  struct sockaddr_in peer_addr;
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_addr.s_addr = inet_addr(argv[3]);
  peer_addr.sin_port = htons((unsigned short) atoi(argv[4]));

  if (bind(my_descriptor, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
    printf("Failed to bind %s\n", argv[1]);
    close(my_descriptor);
    return -1;
  }

  int wait_for = get_remaining_msec();
  printf("Waiting for %d microseconds\n", wait_for);
  usleep(wait_for);

  printf("Connecting...\n");
  if (connect(my_descriptor, (struct sockaddr*) &peer_addr, sizeof(peer_addr)) < 0) {
    printf("Connection Attempt Failed\n");
    return -1;
  }
  printf("Connection Established\n");
  if (write(my_descriptor, message, sizeof(message)) < 0) {
    printf("Failed to send message");
    return -1;
  }
  char* buffer = malloc(BUF_SIZE);
  if (read(my_descriptor, buffer, sizeof(buffer)) < 0) {
    printf("Failed to read message");
    free(buffer);
    return -1;
  }
  printf("Received: %s \n", buffer);
  free(buffer);
  close(misc_descriptor);
  close(my_descriptor);

  return 0;
}

int get_remaining_msec() {
  struct timeval my_time;
  gettimeofday(&my_time, NULL);
  struct tm tm;
  gmtime_r(&my_time.tv_sec, &tm);
  int sec = 60 - tm.tm_sec - 1;
  int ms = 1000000 - my_time.tv_usec;
  return sec * 1000000 + ms;
}
