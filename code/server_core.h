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


int OPTION = 1;
int MAX_CONN_REQEUSTS = 10;
int MAX_DONORS = 20;

int send_buffer_size = 2048;
int recv_buffer_size = 2048;

void* s_add_requestor(unsigned long r_IP);
void* s_add_donor(unsigned long d_IP);
void* s_remove_requestor(unsigned long r_IP);
void* s_remove_donor(unsigned long d_IP);
