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
  puts("Server Message: ");
  print_message(receive_buffer);
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

int main(void) {
  int sockfd=0, p=0, n=0, i, PORT;
  struct sockaddr_in serv_addr;
  char * my_message = malloc(send_buffer_size );
  char IP_ADDR[22];
  char str_port[6];
  char c;
  clock_t t;

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

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

  become_requestor(serv_addr);
  strcpy(my_message, "What the fuck did you just fucking say about me, you little bitch? I'll have you know I graduated top of my class in the Navy Seals, and I've been involved in numerous secret raids on Al-Quaeda, and I have over 300 confirmed kills. I am trained in gorilla warfare and I'm the top sniper in the entire US armed forces. You are nothing to me but just another target. I will wipe you the fuck out with precision the likes of which has never been seen before on this Earth, mark my fucking words. You think you can get away with saying that shit to me over the Internet? Think again, fucker. As we speak I am contacting my secret network of spies across the USA and your IP is being traced right now so you better prepare for the storm, maggot. The storm that wipes out the pathetic little thing you call your life. You're fucking dead, kid. I can be anywhere, anytime, and I can kill you in over seven hundred ways, and that's just with my bare hands. Not only am I extensively trained in unarmed combat, but I have access to the entire arsenal of the United States Marine Corps and I will use it to its full extent to wipe your miserable ass off the face of the continent, you little shit. If only you could have known what unholy retribution your little 'clever' comment was about to bring down upon you, maybe you would have held your fucking tongue. But you couldn't, you didn't, and now you're paying the price, you goddamn idiot. I will shit fury all over you and you will drown in it. You're fucking dead, kiddo.");
  unsigned int ID = store(serv_addr, my_message);
  free(my_message);
  //We can now do stuff with all our extra memory
  //We then load it back
  t = clock();
  char* new_message = retrieve(serv_addr, ID);
  t = clock() - t;
  double time_taken = 1000.0 * ((double)t)/CLOCKS_PER_SEC;
  printf("Time to get from network(ms): %f\n", time_taken);
  printf("(Est. Avg)Time from disk(ms): 5\n");
  puts("Server Message: ");
  print_message(new_message);

  return 0;
}
