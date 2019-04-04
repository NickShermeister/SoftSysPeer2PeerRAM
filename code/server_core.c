
struct{
  int id;
  int size;
  int d_ip;
} location;

struct {
  int r_size;
  int d_size;
  int num_ids;
  IP* requesters;
  IP* donors;
  location* ids;
}server;

server* s_start(){
  //TODO
}

void* s_free(server* s){
  for(int i =0;i < s->num_ids;i++){
    free(ids[i]);
  }
  free(s)
}

void* s_add_requestor(IP* r_IP){
  //TODO
}

void* s_add_donor(IP* d_IP){
  //TODO
}

void* s_remove_requestor(IP* r_IP){
  //TODO
}

void* s_remove_donor(IP* d_IP){
  //TODO
}

void* store(void* Data, int size){
  //TODO
}

void* retrieve(int id){
  location* c_id = NULL;
  for(int i =0;i < s->num_ids;i++){
    if(id == ids[i]->id){
      c_id = id[i];
      break;
    }
  }
  if(c_id == NULL){
    return NULL;
  }
  //Make a request to the right IP address.
}
