#include "server_core.h"

void print_message(char *message) {
  int i;

  for(i=0; i<strlen(message); i++) {
    if (message[i] != '\\') {
      printf("%c", message[i]);
    } else {
      break;
    }
  }

  puts("\n");
}

int main(void)
{
  int sockfd = 0,n = 0;
  char recvBuff[1024];
  char sendBuff[1024] = "Hi from the client.\n\0";
  char receive_buffer[1024];
  struct sockaddr_in serv_addr;

  memset(receive_buffer, '0' ,sizeof(receive_buffer));
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
    puts("Error: Connection Failed\n");
    return 1;
  } else {
    puts("Connection Successful\n");
  }

  if(recv(sockfd, receive_buffer, 1024, 0) < 0) {
    puts("Receive failed\n");
    return 1;
  }

  puts("Server Message:");
  print_message(receive_buffer);

  return 0;
}
