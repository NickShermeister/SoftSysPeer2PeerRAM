#include "server_core.h"

void print_ip() {
  FILE *fp;
  char ips[1024];
  char temp[1024];
  int read = 0;
  int i;

  fp = popen("ip address | grep -v '127' | grep 'inet 10\\|inet 172\\|inet 192'", "r");

  while(fgets(ips, sizeof(ips)-1, fp) != NULL) {

    for(i=0; i<strlen(ips); i++) {

      if(!read && ips[i] == '1') {
        read = 1;
      } else if(read && ips[i] == '/') {
        temp[read-1] = '\0';
        read = 0;
        break;
      }

      if(read) {
        temp[read-1] = ips[i];
        read++;
      }
    }

    printf("%s:%d\n", temp, 5000);
  }

  pclose(fp);
}

int main(void)
{
  int listenfd = 0, connfd = 0;
  int client_count = 0;

  struct sockaddr_in serv_addr;
  struct sockaddr *client_address;
  int size = sizeof(serv_addr);

  char send_buffer[1024];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &OPTION, sizeof(OPTION));

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(send_buffer, '0', sizeof(send_buffer));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
    puts("Failed to listen\n");
    return -1;
  }

  puts("Server IP:");
  print_ip();
  puts("\n");

  while(1) {
    connfd = accept(listenfd, (struct sockaddr *) &client_address, &size);
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*) &client_address;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];

    printf("%s\n", inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN ));
    client_count++;
    puts("New Client Connection\n");
    printf("Total Connections:%d\n\n", client_count);

    strcpy(send_buffer, "Wowzers\\");
    write(connfd, send_buffer, strlen(send_buffer));

    close(connfd);
    sleep(1);
  }

  return 0;
}
