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

int PORT = 5000;
char* IP_ADDR = "192.168.32.209"; //Edit to your IP
int OPTION = 1;
int MAX_CONN_REQEUSTS = 10;
int MAX_DONORS = 20;

void* s_add_requestor(void* r_IP);
void* s_add_donor(void* d_IP);
void* s_remove_requestor(void* r_IP);
void* s_remove_donor(void* d_IP);
