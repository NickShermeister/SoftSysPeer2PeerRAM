#include "hashmap.h"
#include "server_core.h"
#include "general_functions.c"

typedef struct {
  hashmap* requestors;
  hashmap* donors;
  hashmap* locations;
}server;

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
server* s;
void* (*f[])(void*) = {&s_add_requestor, &s_add_donor, &s_remove_requestor, &s_remove_donor};
pthread_mutex_t requestor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t donor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t location_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;

int IDNO = 100;
int send_buffer_size = 2048;
int recv_buffer_size = 2048;



void accept_request(int index, int IP){
  //process type of request
  //TODO
  //Optionally spawn child thread to handle request
  pthread_t thread_id;
  int *arg = malloc(sizeof(*arg));
  *arg = IP;
  pthread_create(&thread_id, NULL, f[index], arg);
  //TODO
}

// void* fooAPI(void* param) {
//   pthread_mutex_lock(&mutex);
//   printf("Changing the shared resource now.\n");
//   sharedResource = 42;
//   pthread_mutex_unlock(&mutex);
//   return 0;
// }

//All of the following should optionally be allowed to be executed in a separate thread

void* s_add_requestor(unsigned long r_IP){
  // int r_IP_int = *((unsigned long *) r_IP);

  //Should block requestor validation
  pthread_mutex_lock(&requestor_mutex);
  //Add requestor to the list of valid requestors
  insert(s->requestors, r_IP, 0);
  pthread_mutex_unlock(&requestor_mutex);
  return NULL;
}

void* s_add_donor(unsigned long d_IP){
  // int d_IP_int = *((unsigned long *) d_IP);
  //Should block donor choice and request sending
  pthread_mutex_lock(&donor_mutex);
  //Add donor to the list
  insert(s->donors, d_IP, 0);
  pthread_mutex_unlock(&donor_mutex);
  return NULL;
}

void* s_remove_requestor(unsigned long r_IP){
  // int r_IP_int = *((unsigned long *) r_IP);
  //Should block requestor validation
  pthread_mutex_lock(&requestor_mutex);
  //Remove requestor from list of valid requestors
  delete(s->requestors, r_IP);
  pthread_mutex_unlock(&requestor_mutex);
  return NULL;

}

void* s_remove_donor(unsigned long d_IP){
  // int d_IP_int = *((unsigned long *) d_IP);
  //Should block donor validation
  pthread_mutex_lock(&donor_mutex);
  //Remove donor from list of valid donor
  delete(s->requestors, d_IP);
  pthread_mutex_unlock(&donor_mutex);
  return NULL;
}

void store(int r_IP, void* Data, int size){
  //Validate requestor
  if(search(s->requestors, r_IP) == NULL){
    //We don't know who you are
    printf("Requestor not recognized: %i\n", r_IP);
    return;
  }

  //Generate id
  int id = IDNO;
  IDNO++;
  //Choose Donor from the list

  //TODO do this better
  DataItem* donor = first(s->donors);
  if(donor == NULL){
    //no donors :()
    return;
  }
  int d_IP = donor->key;
  //Send request to donor
  //TODO by Nick and Nate
  //Save donor for id
  insert(s->locations, id, d_IP);
}

void retrieve(int r_IP, int id){
  // Validate requestor
  if (search(s->requestors, r_IP) == NULL) {
    return;
  }
  //Retrieve donor
  DataItem* loc = search(s->locations, id);
  //Validate donor
  if (search(s->donors, loc->data) == NULL) {
    return;
  }
  //Make a request to the right IP address.
  //TODO for somebody and Nate
  //Return the data
  //TODO
}


int listenfd = 0, connfd = 0;
int client_count = 0;
struct sockaddr_in serv_addr;
struct sockaddr *client_address;
int size = sizeof(serv_addr);

int setup(){
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &OPTION, sizeof(OPTION));

  memset(&serv_addr, '0', sizeof(serv_addr));
  // memset(send_buffer, '0', sizeof(send_buffer));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if(listen(listenfd, MAX_CONN_REQEUSTS) == -1){
    puts("Failed to listen\n");
    return -1;
  }

  puts("Server IP:");
  print_ip();
  puts("\n");

  return 0;

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
  pthread_mutex_unlock(socketMutex)
  //Okay now we can modify arg and client_address

  // printf("newSocket: %d\n", newSocket);
  recv(newSocket , recv_buffer , recv_buffer_size , 0);
  printf("Received: %s\n", recv_buffer);

  //Do different work depending on message recieved
  //TODO
  int mode = ((int)recv_buffer[0]) - 48;
  if(mode < 4){//we are adding or deleting ip address
      f[mode](ipAddr.s_addr);
  }else if (mode = 4){
      //TODO
  }else if(mode = 5){
    //TODO
  }else{
    printf("Uh oh. Couldnt find method");
  }
  // Send message to the client socket
  // strcpy(send_buffer, "WowzersMKII\\");
  write(newSocket,send_buffer,strlen(send_buffer));
  //Exit
  printf("Exit socketThread \n");
  close(newSocket);
  pthread_exit(NULL);
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
    // connfd = accept(listenfd, (struct sockaddr *) &client_address, &size);
    pthread_mutex_lock(socketMutex)
    connfd = accept(listenfd, (struct sockaddr *) &client_address, &size);
    // printf("connfd: %d\n", connfd);
    if( pthread_create(&tid[i], NULL, socketThread, &connfd) != 0 )
       printf("Failed to create thread\n");
       pthread_mutex_unlock(socketMutex)


    // printf("%s\n", inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN ));
    client_count++;
    puts("New Client Connection\n");
    printf("Total Connections:%d\n\n", client_count);

    sleep(1);
  }
  return 0;
}


int main(){
  s = s_start();
  int valid_setup = setup();
  if(valid_setup != 0){
    printf("Something went wrong with the server setup. Shutting down now.");
    return -1;
  }

  server_main();

  int donor_name_1 = 2001;
  int donor_name_2 = 2002;
  int req_name_1 = 1001;
  int req_name_2 = 1002;
  accept_request(1,donor_name_1);
  accept_request(1,donor_name_2);
  accept_request(0,req_name_1);
  accept_request(0,req_name_2);
  sleep(1);
  store(req_name_1, NULL, 0);
  store(req_name_2, NULL, 0);
  store(87, NULL, 0);
  display(s->locations);
  return 0;
}
