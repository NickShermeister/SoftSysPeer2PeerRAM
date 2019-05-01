#include "server_core.h"
#include "general_functions.c"

server* s_start(){
  server* s = malloc(sizeof(server));
  s->requestors = declare_map(hashCode);
  s->donors = declare_map(hashCode);
  s->locations = declare_map(hashCode);
  return s;
}

// don't want to get paddled
void* s_free(server* s){
  free_map(s->requestors);
  free_map(s->donors);
  free_map(s->locations);
  free(s);
}

typedef void* (*f_array_type)(unsigned int   );
f_array_type f_array[3] = {&s_add_requestor, &s_remove_requestor, &s_remove_donor};
pthread_mutex_t requestor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t donor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t location_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;

int IDNO = 100;
int listenfd = 0, connfd = 0;
int client_count = 0;
struct sockaddr_in serv_addr;
struct sockaddr *client_address;
int size = sizeof(serv_addr);

//All of the following should optionally be allowed to be executed in a separate thread

void* s_add_requestor(unsigned int    r_IP){
  //Should block requestor validation
  pthread_mutex_lock(&requestor_mutex);
  //Add requestor to the list of valid requestors

  printf("in add requestor\n");
  fflush(stdout);
  insert(running_server->requestors, r_IP, NULL);
  pthread_mutex_unlock(&requestor_mutex);
  return NULL;
}

void* s_remove_requestor(unsigned int    r_IP){
  // int r_IP_int = *((unsigned int    *) r_IP);
  //Should block requestor validation
  printf("in add req\n");
  pthread_mutex_lock(&requestor_mutex);
  //Remove requestor from list of valid requestors
  delete(running_server->requestors, r_IP);
  pthread_mutex_unlock(&requestor_mutex);
  return NULL;

}

void* s_remove_donor(unsigned int    d_IP){
  // int d_IP_int = *((unsigned int    *) d_IP);
  //Should block donor validation
  pthread_mutex_lock(&donor_mutex);
  //Remove donor from list of valid donor
  delete(running_server->donors, d_IP);
  pthread_mutex_unlock(&donor_mutex);
  return NULL;
}

void* s_add_donor(unsigned int d_IP, unsigned int port){
  // int d_IP_int = *((unsigned int    *) d_IP);
  //Should block donor choice and request sending
  pthread_mutex_lock(&donor_mutex);
  //Add donor to the list
  unsigned int * p = malloc(sizeof(unsigned int));
  *p = port;
  insert(running_server->donors, d_IP, (void *)p);
  pthread_mutex_unlock(&donor_mutex);
  return NULL;
}

void store(unsigned int r_IP, unsigned int r_port_number, char* recv_buffer){
  char id_str[10];
  char send_buffer[send_buffer_size];
  int c_size = sizeof(char);
  //Validate requestor
  if(search(running_server->requestors, r_IP) == NULL){
    //We don't know who you are
    printf("Requestor not recognized: %i\n", r_IP);
    return;
  }

  //Generate id
  unsigned int id = IDNO;
  IDNO++;//not thread safe but it's fine probably
  //Choose Donor from the list

  //TODO do this better
  DataItem* donor = first(running_server->donors);
  if(donor == NULL){
    //no donors :()
    return;
  }
  unsigned int * p = malloc(sizeof(unsigned int));
  *p = donor->key;
  unsigned int d_port = *(unsigned int *)(donor->data);
  //Save donor for id
  insert(running_server->locations, id, p);
  strcpy(send_buffer, "1\0");//Mode 1 for donors says we're sending data
  //send id back to whoever
  my_itoa(id, id_str);
  id_str[3] = '\0';
  strcpy(send_buffer + (c_size*2) , id_str);
  strcpy(send_buffer+6*c_size, recv_buffer + 2* c_size);

  //Send request to donor
  // printf("Our message: %s \n",recv_buffer);
  // printf("Our buffer: %s \n",send_buffer);
  // printf("Our port: %u \n",d_port);
  if(write(d_port,send_buffer,6 + strlen(send_buffer+6*c_size))<0){
    printf("Write failed------------------\n\n");
  }
  recv(d_port , send_buffer , 14 , 0);
  write(r_port_number,id_str,strlen(id_str));

}

void retrieve(unsigned int r_IP, unsigned int r_port_number, char* message){
  char send_buffer[send_buffer_size];
  char id_str[10];
  unsigned int id = atoi(message + 2);
  // Validate requestor
  if (search(running_server->requestors, r_IP) == NULL) {
    printf("Requestor not recognized: %i\n", r_IP);
    return;
  }
  //Retrieve donor
  DataItem* loc = search(running_server->locations, id);
  //Validate donor
  DataItem* donor_info = search(running_server->donors, *(unsigned int *)loc->data);
  if (donor_info == NULL) {
    return;
  }
  //Make a request to the right IP address.
  unsigned int d_port = *(unsigned int *)(donor_info->data);
  strcpy(send_buffer, "2\0");//Mode 2 for donors says we're retrieving data
  strcpy(send_buffer + (sizeof(char)*2) , message+2);
  send_buffer[5] = '\0';
  send_buffer[6] = '\\';
  write(d_port,send_buffer,7);
  //Wait on the data from the donor
  recv(d_port , send_buffer , send_buffer_size , 0);
  //Return the data
  write(r_port_number, send_buffer,strlen(send_buffer));
}

void delete_all_data(){
  // printf("In delete data\n\n\n\n\n");

  hashmap* hm = running_server->donors;
  char send_buffer[send_buffer_size];
  strcpy(send_buffer, "3\\");
  DataItem* item;
  for(int i =0; i< hm->arr_size; i++){
    item = hm->hash_array[i];
    if(item != NULL && item->key != 0){
      write(*(unsigned int *)(item->data),send_buffer,3);
    }
  }

  free_map(running_server->locations);
  running_server->locations = declare_map(hashCode);
  IDNO = 100;
  printf("Reset-----------------------------------------\n");
}

/*
  Thread entry that handles what happens when we get a new connection. Currently
*/

void * socketThread(void *arg)
{
  //Define the important local variabls
  struct sockaddr_in* pV4Addr;
  struct in_addr ipAddr;
  char send_buffer[send_buffer_size];
  char recv_buffer[recv_buffer_size];
  //Copy the important information
  pV4Addr = (struct sockaddr_in*) &client_address;
  ipAddr = pV4Addr->sin_addr;
  int newSocket = *((int *)arg);
  //Okay now we can modify arg and client_address
  pthread_mutex_unlock(&socket_mutex);
  //Do the actual work
  // printf("newSocket: %d\n", newSocket);
  recv(newSocket , recv_buffer , recv_buffer_size , 0);
  // printf("Received: %s\n", recv_buffer);
  //Do different work depending on message recieved
  int mode = ((int)recv_buffer[0]) - 48;
  switch(mode){
    case 0:
    case 1:
    case 2:
      //this means we only have to interpret one Message
      //maybe we want to send a confirmation
      f_array[mode](ipAddr.s_addr);
      strcpy(send_buffer, "Did the thing\\");
      write(newSocket,send_buffer,strlen(send_buffer));
      close(newSocket);
      break;
    case 3:
      s_add_donor(ipAddr.s_addr, (unsigned int)newSocket);
      break;
    case 4:
      //Store
      //Since we are storing we may have to accept an arbitrary amount of messages
      store(ipAddr.s_addr, (unsigned int)newSocket, recv_buffer);
      close(newSocket);
      break;
    case 5:
      //Retrieve
      retrieve(ipAddr.s_addr, (unsigned int)newSocket, recv_buffer);
      close(newSocket);
      //Since we are retrieving we may have to send an arbitrary amount of packets
      //TODO
      break;
    case 6:
      //for testing purposes
      //delete all locations
      delete_all_data();
      write(newSocket, send_buffer,1);
      close(newSocket);
      break;
    default:
      printf("Uh oh. Couldnt find method");
      printf("Received: %s\n", recv_buffer);
      // close(newSocket);
  }
  // display(running_server->requestors);
  // display(running_server->donors);
  // display(running_server->locations);
  // Send message to the client socket
  //Exit
  pthread_exit(NULL);
}

int setup(){
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &OPTION, sizeof(OPTION));
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);
  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if(listen(listenfd, MAX_CONN_REQEUSTS) == -1){
    puts("Failed to listen\n");
    return -1;
  }
  puts("Server IP:");
  print_ip(5000);
  puts("\n");
  return 0;
}

int server_main(){
  int success;
  char str[INET_ADDRSTRLEN];
  struct sockaddr_storage serverStorage;
  int serverSocket, newSocket;
  pthread_t tid[MAX_CONN_REQEUSTS];
  pthread_t donors[MAX_DONORS];
  int i = 0;
  while(1){
    pthread_mutex_lock(&socket_mutex);
    printf("Waiting for acceptance\n");
    connfd = accept(listenfd, (struct sockaddr *) &client_address, &size);
    printf("connfd: %d\n", connfd);
    if( pthread_create(&tid[i], NULL, socketThread, &connfd) != 0 ){
       printf("Failed to create thread\n");
       pthread_mutex_unlock(&socket_mutex);
    }
    // printf("%s\n", inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN ));
    client_count++;
    puts("New Client Connection\n");
    printf("Total Connections:%d\n\n", client_count);
  }
  return 0;
}


int main(){

  // Adjust stack size
  running_server = s_start();
  int valid_setup = setup();
  if(valid_setup != 0){
    printf("Something went wrong with the server setup. Shutting down now.");
    return -1;
  }
  server_main();
  return 0;
}
