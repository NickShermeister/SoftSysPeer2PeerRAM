#include "server_core.h"


int main(void)
{
  int sockfd = 0,n = 0;
  char recvBuff[1024];
  char sendBuff[1024] = "Hi from the client.\n\0";
  struct sockaddr_in serv_addr;

  memset(recvBuff, '0' ,sizeof(recvBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
      printf("\n Error : Could not create socket \n");
      return 1;
    }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
      printf("\n Error : Connect Failed \n");
      return 1;
    }

  send(sockfd, sendBuff, strlen(sendBuff), 0);

  n = read(sockfd, recvBuff, sizeof(recvBuff)-100);
  printf("RecvBuff: %s", recvBuff);

  if( n < 0)
    {
      printf("\n Read Error \n");
    }

  return 0;
}
