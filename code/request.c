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

void become_requestor(struct sockaddr_in serv_addr){
  //Add self as requestor
  char send_buffer[send_buffer_size];
  char receive_buffer[recv_buffer_size];
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //Connect
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  //Sending 0 adds IP as requester
  strcpy(send_buffer, "0\0");
  write(sockfd, send_buffer, strlen(send_buffer));
  //Wait for confirmation(optional)
  if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
    puts("Receive failed\n");
    return;
  }
  puts("Server Message: ");
  print_message(receive_buffer);
  close(sockfd);
}

unsigned int store(struct sockaddr_in serv_addr,char* stored_string){
  //Save message
  char send_buffer[send_buffer_size];
  char receive_buffer[recv_buffer_size];
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //connect
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  //Mode 4 means store request
  strcpy(send_buffer, "4_");
  //Copy the string to store into the buffer
  strcpy(send_buffer+2, stored_string);
  // End with \\ to represent end of buffer
  send_buffer[2+strlen(stored_string)] = '\\';
  write(sockfd, send_buffer, strlen(send_buffer));
  //Wait to get the ID number
  if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
    puts("Receive failed\n");
    return 0;
  }
  // puts("Server Message: ");
  // print_message(receive_buffer);
  //Parse and return ID number
  unsigned int file_id = atoi(receive_buffer);
  close(sockfd);
  return file_id;
}

char* retrieve(struct sockaddr_in serv_addr, unsigned int ID){
  //Request Message
  char send_buffer[send_buffer_size];
  char* receive_buffer = malloc(recv_buffer_size);
  char id_str[5];
  //Connect to socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  //5 means we are requesting data
  strcpy(send_buffer, "5_");
  //turn the ID into a string of size 3
  my_itoa(ID, id_str);
  //Cut off at 3 and add end of buffer
  id_str[3] = '\0';
  id_str[4] = '\\';
  strcpy(send_buffer + 2, id_str);
  // print_message(send_buffer);
  write(sockfd, send_buffer, 7);
  //Wait for the data back
  if(recv(sockfd, receive_buffer, recv_buffer_size, 0) < 0) {
    puts("Receive failed\n");
    return NULL;
  }
  // puts("Server Message: ");
  // print_message(receive_buffer);
  close(sockfd);
  return receive_buffer;
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

double test_speed_local(int str_length, int number_values){
  char* p[number_values];
  double results[number_values];
  printf("In local\n");
  for(int i = 0; i<number_values;i++){
    p[i] = malloc(sizeof(char) * str_length);
  }
  clock_t t;
  printf("Mallocs\n");

  for(int i = 0; i<number_values;i++){
    t = clock();
    (p[i])[str_length-1] = 'a';
    t = clock() - t;
    results[i] = 1000.0 * ((double)t)/CLOCKS_PER_SEC;
  }
  printf("accessed\n");
  for(int i = 0; i<number_values;i++){
    free(p[i]);
  }
  printf("freed\n");
  double total = 0;
  for(int i = 0; i<number_values;i++){
    total += results[i];
  }
  printf("before return\n");
  return total/(double)number_values;
}

double test_speed(struct sockaddr_in serv_addr, int str_length, int number_values){
  double results[number_values];
  unsigned int ids[number_values];
  char send_buffer[send_buffer_size];
  char receive_buffer[recv_buffer_size];

  if(number_values >= 900){
    printf("sorry number capped right now");
    return 0;
  }else if(str_length>=recv_buffer_size-3){
    printf("sorry str len capped right now");
    return 0;
  }

  int ID = 100;
  for(int i = 0; i<number_values;i++){
    my_itoa(ID, send_buffer);
    for(int j = 3; j<str_length; j++){
      send_buffer[j] = 'a';
    }
    send_buffer[str_length] = '\0';
    send_buffer[str_length+1] = '\\';
    ids[i] = store(serv_addr, send_buffer);
    ID++;
  }
  // printf("Data Stored\n");
  clock_t t;

  for(int i = 0; i<number_values;i++){
    t = clock();
    char* new_message = retrieve(serv_addr, ids[i]);
    t = clock() - t;
    results[i] = 1000.0 * ((double)t)/CLOCKS_PER_SEC;
    // print_message(new_message);
  }

  double total = 0;
  for(int i = 0; i<number_values;i++){
    // printf("%f\n",results[i]);
    total += results[i];
  }
  printf("Len %d: %f\n", str_length, total/(double)number_values);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  strcpy(send_buffer,"6\\");
  write(sockfd, send_buffer, 3);
  recv(sockfd , send_buffer , 1 , 0);
  close(sockfd);
  return total/(double)number_values;
}


int main(void) {
  char * my_message = malloc(send_buffer_size );
  clock_t t;
  struct sockaddr_in* serv_addr = malloc(sizeof(struct sockaddr_in));
  get_IP(serv_addr);

  become_requestor(*serv_addr);

  int max_ = 1500;
  int min_ = 10;
  int inc = 50;
  int size =1 + (max_ - min_)/inc;
  double results[size + 10];
  double results_local[size + 10];
  int index = 0;//I dont want to do the math
  for(int i = min_; i < max_; i+=inc){
    results[index] = test_speed(*serv_addr, i, 400);
    results_local[index] = test_speed_local(i, 400);
    index++;
  }
  printf("Final index: %d",index);

  FILE * fp, *fp2;
  int i;
  /* open the file for writing*/
  fp = fopen ("./results.txt","w");
  fp2 = fopen ("./results_local.txt","w");

  /* write 10 lines of text into the file stream*/
  for(i = 0; i < index;i++){
    fprintf (fp, "%f\n", results[i]);
    fprintf (fp2, "%f\n", results_local[i]);
  }

  /* close the file*/
  fclose (fp);
  fclose (fp2);

  return 0;
}
