#include "server_core.h"
#include  <signal.h>

static volatile int keepRunning;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(void)
{
  int listenfd = 0,connfd = 0;
  keepRunning = 1;

  // struct sigaction act;
  // act.sa_handler = intHandler;
  // sigaction(SIGINT, &act, NULL);
  signal(SIGINT, intHandler);


  struct sockaddr_in serv_addr;

  char sendBuff[1025];
  int numrv;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);


  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
  }

  printf("Before loop\n.");

  do
    {
      printf("My IP is: \n");
      system("ifconfig | grep -A 2 'wlp'");
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

      strcpy(sendBuff, "Message from server");
      write(connfd, sendBuff, strlen(sendBuff));

      close(connfd);
      sleep(1);
    } while(keepRunning == 1);


    close(connfd);
    printf("Exiting successfully.\n");
  return 0;
}
