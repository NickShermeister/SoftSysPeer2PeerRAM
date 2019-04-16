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

int OPTION = 1;
int MAX_CONN_REQEUSTS = 10;
int MAX_DONORS = 20;

void* s_add_requestor(unsigned long r_IP);
void* s_add_donor(unsigned long d_IP);
void* s_remove_requestor(unsigned long r_IP);
void* s_remove_donor(unsigned long d_IP);
