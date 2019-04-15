/*
  Used for functions that the client or the server may want for some reason.
*/

#include "general_functions.h"

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

    printf("%s:%d\n", temp, PORT);
  }

  pclose(fp);
}
