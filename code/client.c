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


int main(void) {
  int sockfd=0, p=0, n=0, i, PORT;
  char receive_buffer[recv_buffer_size];
  struct sockaddr_in serv_addr;
  char * send_buffer = malloc(send_buffer_size * (sizeof(char)));
  // [send_buffer_size] = "2: Hi!\0";
  char IP_ADDR[22];
  char str_port[6];
  char c;

  memset(receive_buffer, '0' ,sizeof(receive_buffer));
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
    puts("Error: Connection Failed\n");
    return 1;
  } else {
    puts("Connection Successful\n");
  }

  // puts("What mode do you wish to be in? \n0: Add Requestor\n1: Remove Requestor\n2: Add Donor\n3: Remove Donor \n");
  // // int success;
  // fgets(send_buffer, 10, stdin);

  // send_buffer[0] = c;
  // send_buffer[1] = '\0';
  char inputs_[] = {'0','1','2','3'};
  for(int i = 0; i <4; i++){
    send_buffer[0] = inputs_[i];
    if(write(sockfd, send_buffer, strlen(send_buffer)) < 0){
      puts("Send failed.\n");
      return 1;
    }

    puts("Send success.\n");

    if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
      puts("Receive failed\n");
      return 1;
    }
    puts("Server Message: ");
    print_message(receive_buffer);
    sleep(1);
    
  }



  char fname[100];
  FILE* fp;
  memset(fname, 0, 100);
  /*ask user for the name of the file*/
  char* fileprompt = "Enter file name: ";
  puts(fileprompt);
  fgets(fname, 100, stdin);

  // puts(fname);
  strtok(fname, "\n");

  //Opens the file from where the text will be read.
  fp = fopen(fname, "r");

  if (fp == NULL)
    {
        puts("Error, Unable to open the file for reading.\n");
        return 1;
    }

    char ch;
    while((ch = fgetc(fp)) != EOF){
      printf("%c", ch);
    }

  fclose(fp);

  puts("File closed. Shutting down client.");

  free(send_buffer);

  return 0;
}
