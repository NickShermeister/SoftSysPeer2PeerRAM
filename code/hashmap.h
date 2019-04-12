

typedef struct {
   int data;
   int key;
}DataItem;

typedef struct{
  DataItem** hash_array;
  int arr_size;
  float num_elem;
  int (*hashcode) (int, int);
}hashmap;

extern int DEFAULT_SIZE;
DataItem* dummyItem;
DataItem* item;

void rehash(hashmap* hm, int dir);
void display(hashmap* hm);
DataItem* delete(hashmap* hm,  int key);
void insert(hashmap* hm,int key,int data);
DataItem *search(hashmap* hm, int key);
hashmap* declare_map(int (*hashcode)(int, int));
int hashCode(int size, int key);
void free_map(hashmap* hm);
DataItem *first(hashmap* hm);
