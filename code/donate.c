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

void get_IP(struct sockaddr_in* serv_addr){
  int sockfd=0, p=0, n=0, i, PORT;
  char IP_ADDR[22];
  char str_port[6];
  char c;

  //Parse the IP
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

  serv_addr->sin_family = AF_INET;
  serv_addr->sin_port = htons(PORT);
  serv_addr->sin_addr.s_addr = inet_addr(IP_ADDR);
}

void become_donor(struct sockaddr_in serv_addr){
  //Add self as donor
  char send_buffer[send_buffer_size];
  // char receive_buffer[recv_buffer_size];
  puts("Server Message: ");
  print_message(receive_buffer);
  close(sockfd);

  //Add self as donor
  //Declare socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //Inform server that you exist
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  //Inform the server that you are a donor
  strcpy(send_buffer, "3");
  write(sockfd, send_buffer, strlen(send_buffer));
}

store(hashmap* hm, char* recv_buffer){
  int skip_for_data= 6;
  //Accepting data
  //Get the id number first
  int ID = atoi(receive_buffer+2*(sizeof(char)));
  //Get where the actual information starts
  char* data_start = receive_buffer + skip_for_data * sizeof(char);
  int string_size = strlen(data_start);
  //Save a copy so we can service other requests
  char* data_copy = malloc(sizeof(char) * (string_size+1));
  strcpy(data_copy, data_start);
  //Put copy in hashmap under the key ID
  insert(hm, ID, (void*) data_copy);
  // printf("Save:\nID:%u\n", ID);
  write(sockfd, "Did the thing", 14);

  // printf("%s\n", data_copy);
}

retrieve(hashmap* hm, char* recv_buffer){
  //Sending data back
  //Again parse ID
  int ID = atoi(receive_buffer+2*(sizeof(char)));
  // printf("Load:\nID: %u", ID);
  DataItem* d = search(hm, ID);
  if(d==NULL){
    printf("Yikes");
    send_buffer[0] = '\0';
    write(sockfd, send_buffer, 1);
    continue;
  }
  //Get the pointer
  char* stored_data = (char*)d->data;
  write(sockfd, stored_data, strlen(stored_data));
}

int main(void) {
  //We need a hashmap to find stuf given an ID
  hashmap* hm = declare_map(hashCode);
  //various helpful variables
  int sockfd=0, p=0, n=0, i, PORT;
  char receive_buffer[recv_buffer_size];
  struct sockaddr_in serv_addr;
  char * send_buffer = malloc(send_buffer_size * (sizeof(char)));
  get_IP(&serv_addr);
  become_donor(serv_addr);
  //Clear our buffer
  memset(receive_buffer, '\0' ,sizeof(receive_buffer));
  //Actually accept requests
  while(1){
    //Clear memory from last request
    memset(receive_buffer, '\0' ,sizeof(receive_buffer));
    if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
      printf("Receive failed: port %d\n", sockfd);
    }else{
      //We successfully filled the buffer
      // printf("Server port: %d\n", sockfd);
      //Parse the mode
      int mode = ((int)receive_buffer[0]) - 48;
      // printf("Mode: %d\n",mode);
      switch(mode){
        case 1:
          store(hm, receive_buffer);
          break;
        case 2:
          retrieve(hm, receive_buffer);
          break;
        case 3:
          //for testing purposes. Forget what we know
          printf("Case 3");
          free_map(hm);
          hm = declare_map(hashCode);
          printf("Reset----------");
          break;
        default:
          printf("Couldnt parse data");
      }
    }
  }
  return 0;
}
