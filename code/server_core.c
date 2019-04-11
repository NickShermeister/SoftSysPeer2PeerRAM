#include "hashmap.h"


typedef struct {
  hashmap* requesters;
  hashmap* donors;
  hashmap* locations;
}server;

server* s_start(){
  server* s = malloc(sizeof(server));
  s->requesters = declare_map(hashCode);
  s->donors = declare_map(hashCode);
  s->locations = declare_map(hashCode);
  return s;
}

void* s_free(server* s){
  free_map(s->requesters);
  free_map(s->donors);
  free_map(s->locations);
  free(s);
}

void requests[] = {s_add_requestor, s_add_donor, s_remove_requestor, s_remove_donor, store, retrieve};

void* accept_request(){
  //process type of request
  //TODO
  //Optionally spawn child thread to handle request
  //TODO
}

//All of the following should optionally be allowed to be executed in a separate thread

void* s_add_requestor(int r_IP){
  //Should block requestor validation
  //Add requestor to the list of valid requestors
  //TODO
}

void* s_add_donor(int d_IP){
  //Should block donor choice and request sending
  //Add donor to the list
  //TODO
}

void* s_remove_requestor(int r_IP){
  //Should block requestor validation
  //Remove requestor from list of valid requestors
  //TODO
}

void* s_remove_donor(int d_IP){
  //Should block donor choice and request sending
  //Remove donor from the list
  //TODO
  //Remove all appearances of donor?
}

void* store(int r_IP, void* Data, int size){
  //Validate requestor
  //TODO
  //Generate id
  //TODO
  //Choose Donor from the list
  //TODO
  //Send request to donor
  //TODO
  //Save donor for id
  insert(s->locations, id, donor_ip)
  return id;
}

void* retrieve(int r_IP, int id){
  //Validate requestor
  //TODO
  //Retrieve donor location
  DataItem* loc = search(s->locations, id);
  //Validate donor??
  //Make a request to the right IP address.
  //TODO
  //Return the data
  //TODO
}
