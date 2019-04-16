

typedef struct {
   int data;
   unsigned long key;
}DataItem;

typedef struct{
  DataItem** hash_array;
  int arr_size;
  float num_elem;
  int (*hashcode) (int, unsigned long);
}hashmap;

extern int DEFAULT_SIZE;
DataItem* dummyItem;
DataItem* item;

void rehash(hashmap* hm, int dir);
void display(hashmap* hm);
DataItem* delete(hashmap* hm,  unsigned long key);
void insert(hashmap* hm,unsigned long key,int data);
DataItem *search(hashmap* hm, unsigned long key);
hashmap* declare_map(int (*hashcode)(int, unsigned long));
int hashCode(int size, unsigned long key);
void free_map(hashmap* hm);
DataItem *first(hashmap* hm);
