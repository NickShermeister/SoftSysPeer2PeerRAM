

typedef struct {
   unsigned int data;
   unsigned int    key;
}DataItem;

typedef struct{
  DataItem** hash_array;
  int arr_size;
  float num_elem;
  int (*hashcode) (int, unsigned int   );
}hashmap;

extern int DEFAULT_SIZE;
DataItem* dummyItem;
DataItem* item;

void rehash(hashmap* hm, int dir);
void display(hashmap* hm);
DataItem* delete(hashmap* hm,  unsigned int    key);
void insert(hashmap* hm,unsigned int    key,unsigned int data);
DataItem *search(hashmap* hm, unsigned int    key);
hashmap* declare_map(int (*hashcode)(int, unsigned int   ));
int hashCode(int size, unsigned int    key);
void free_map(hashmap* hm);
DataItem *first(hashmap* hm);
