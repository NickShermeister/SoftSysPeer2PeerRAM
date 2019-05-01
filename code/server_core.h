#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "hashmap.h"


int OPTION = 1;
int MAX_CONN_REQEUSTS = 10;
int MAX_DONORS = 20;

typedef struct {
  hashmap* requestors;
  hashmap* donors;
  hashmap* locations;
}server;

server* running_server;
int send_buffer_size = 2048;
int recv_buffer_size = 2048;

void* s_add_requestor(unsigned int    r_IP);
void* s_add_donor(unsigned int    d_IP, unsigned int port);
void* s_remove_requestor(unsigned int    r_IP);
void* s_remove_donor(unsigned int    d_IP);

void my_itoa(int num, char *str)
{
        if(str == NULL)
        {
                return;
        }
        sprintf(str, "%d", num);
}
