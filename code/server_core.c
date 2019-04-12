#include "hashmap.h"
#include "server_core.h"

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
// func* requests = {s_add_requestor, s_add_donor, s_remove_requestor, s_remove_donor, store, retrieve};
pthread_mutex_t requestor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t donor_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t location_mutex = PTHREAD_MUTEX_INITIALIZER;
int IDNO = 100;

void accept_request(){
  //process type of request
  //TODO
  //Optionally spawn child thread to handle request
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

void s_add_requestor(int r_IP){
  //Should block requestor validation
  pthread_mutex_lock(&requestor_mutex);
  //Add requestor to the list of valid requestors
  insert(s->requestors, r_IP, 0);
  pthread_mutex_unlock(&requestor_mutex);
}

void s_add_donor(int d_IP){
  //Should block donor choice and request sending
  pthread_mutex_lock(&donor_mutex);
  //Add donor to the list
  insert(s->donors, d_IP, 0);
  pthread_mutex_unlock(&donor_mutex);
}

void s_remove_requestor(int r_IP){
  //Should block requestor validation
  pthread_mutex_lock(&requestor_mutex);
  //Remove requestor from list of valid requestors
  delete(s->requestors, r_IP);
  pthread_mutex_unlock(&requestor_mutex);

}

void s_remove_donor(int d_IP){
  //Should block donor validation
  pthread_mutex_lock(&donor_mutex);
  //Remove donor from list of valid donor
  delete(s->requestors, d_IP);
  pthread_mutex_unlock(&donor_mutex);
}

void store(int r_IP, void* Data, int size){
  //Validate requestor
  if(search(s->requestors, r_IP) == NULL){
    //We don't know who you are
    printf("Requestor not recognized: %i", r_IP);
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
  return id;
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
int main(){
  s = s_start();
  int donor_name_1 = 2001;
  int donor_name_2 = 2002;
  int req_name_1 = 1001;
  int req_name_2 = 1002;
  s_add_donor(donor_name_1);
  s_add_donor(donor_name_2);
  s_add_requestor(req_name_1);
  s_add_requestor(req_name_2);
  store(req_name_1, NULL, 0);
  store(req_name_2, NULL, 0);
  store(87, NULL, 0);
  display(s->locations);
  return 0;
}
