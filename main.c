#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int main () {
  
  int serverSock;

  if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Could not get server socket file descriptor.");
    return -1;
  }
  
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  return 0;
}
