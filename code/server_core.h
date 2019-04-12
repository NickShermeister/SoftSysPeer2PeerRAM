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

// <<<<<<< HEAD
// int PORT = 1677;
// char* IP_ADDR = "127.0.0.1";
// =======
int PORT = 5000;
char* IP_ADDR = "192.168.32.209"; //Edit to your IP

void* s_add_requestor(void* r_IP);
void* s_add_donor(void* d_IP);
void* s_remove_requestor(void* r_IP);
void* s_remove_donor(void* d_IP);
// >>>>>>> 4c282a7ce085fcc2036e683dbd93eafc7844c589
