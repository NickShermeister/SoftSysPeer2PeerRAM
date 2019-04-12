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

int PORT = 1666;
char* IP_ADDR = "127.0.0.1";

void* s_add_requestor(void* r_IP);
void* s_add_donor(void* d_IP);
void* s_remove_requestor(void* r_IP);
void* s_remove_donor(void* d_IP);
