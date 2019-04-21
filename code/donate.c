#include "server_core.h"

int check_message(char *message) {
  int i;

  for(i=0; i<strlen(message); i++) {
    if (message[i] == '\\') {
      return 1;
    }
  }

  return 0;
}

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


int main(void) {
  int sockfd=0, p=0, n=0, i, PORT;
  char receive_buffer[recv_buffer_size];
  struct sockaddr_in serv_addr;
  char * send_buffer = malloc(send_buffer_size * (sizeof(char)));

  char IP_ADDR[22];
  char str_port[6];
  char c;

  puts("Please Enter IP Address of Server:");
  fgets(IP_ADDR, 21, stdin);

  for(i=0; i<strlen(IP_ADDR); i++) {
    if(p) {
      str_port[p-1] = IP_ADDR[i];
      p++;
    }

    if(!p && IP_ADDR[i] == ':') {
      n = i;
      p = 1;
    }
  }

  IP_ADDR[n] = '\0';

  str_port[p-1] = '\0';
  PORT = strtol(str_port, '\0', 10);

  memset(receive_buffer, '0' ,sizeof(receive_buffer));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

  //Add self as requestor
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  strcpy(send_buffer, "3");
  write(sockfd, send_buffer, strlen(send_buffer));
  if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
    printf("Receive failed: port %d\n", sockfd);
  }else{
    printf("Server port: %d\n", sockfd);
    if(check_message(receive_buffer)){
      print_message(receive_buffer);
      close(sockfd);
      break;
    }

  }



  free(send_buffer);

  return 0;
}
