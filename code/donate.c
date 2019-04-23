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
  hashmap* hm = declare_map(hashCode);
  int skip_for_data= 6;
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

  //Actually accept requests
  while(1){
    if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
      printf("Receive failed: port %d\n", sockfd);
    }else{
      printf("Server port: %d\n", sockfd);
      unsigned int ID;
      int mode = ((int)receive_buffer[0]) - 48;
      switch(mode){
        case 1:
          //Accepting data
          ID = atoi(receive_buffer+2*(sizeof(char)));
          char* data_start = receive_buffer + skip_for_data * sizeof(char);
          int string_size = strlen(data_start);
          char* data_copy = malloc(sizeof(char) * (string_size+1));
          strcpy(data_copy, data_start);
          insert(hm, ID, (void*) data_copy);
          printf("%u\n", ID);

          printf("%s\n", data_copy);
          break;
        case 2:
          //Sending data back
          ID = atoi(receive_buffer+2*(sizeof(char)));
          DataItem* d = search(hm, ID);
          if(d==NULL){
            printf("Yikes");
            close(sockfd);
            return 1;
          }
          char* stored_data = (char*)d->data;
          write(sockfd, stored_data, strlen(stored_data));
          break;
        default:
          printf("Couldnt parse data");
      }
    }
  }
  free(send_buffer);

  return 0;
}
