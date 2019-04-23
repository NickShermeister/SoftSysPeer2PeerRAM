
//Code adapted from  https://www.tutorialspoint.com/data_structures_algorithms/hash_table_program_in_c.htm
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashmap.h"

int DEFAULT_SIZE = 10;

int hashCode(int size, unsigned int    key) {
   return (int) (key % size);
}

hashmap* declare_map(int (*hashcode)(int, unsigned int   )){
      hashmap* hm = malloc(sizeof(hashmap));
      hm->arr_size = DEFAULT_SIZE;
      hm->num_elem = 0;
      hm->hash_array = calloc((hm->arr_size),sizeof(DataItem*));
      hm->hashcode = hashcode;
      return hm;
}

void free_map(hashmap* hm){
  for(int i =0; i< hm->arr_size; i++){
    item = hm->hash_array[i];
    if(item != NULL && item->key != 0){
      free(item);
    }
  }
  free(hm->hash_array);
  free(hm);
}

DataItem *first(hashmap* hm){
  for(int i =0; i< hm->arr_size; i++){
    item = hm->hash_array[i];
    if(item != NULL && item->key != 0){
      return item;
    }
  }
 return NULL;
}

DataItem *search(hashmap* hm, unsigned int    key) {
   //get the hash
   int hashIndex = (hm->hashcode)((hm->arr_size), key);

   //move in array until an empty
   while(((hm->hash_array)[hashIndex]) != NULL) {

      if(((hm->hash_array)[hashIndex])->key == key)
         return ((hm->hash_array)[hashIndex]);

      ++hashIndex;

      //wrap around the table
      hashIndex %= hm->arr_size;
   }

   return NULL;
}

void insert(hashmap* hm,unsigned int    key,void* data) {
   DataItem *item = (DataItem*) malloc(sizeof(DataItem));
   item->data = data;
   item->key = key;
   printf("Shallow into insert\n");
   fflush(stdout);


   //get the hash
   int hashIndex = (hm->hashcode)((hm->arr_size), key);
   //move in array until an empty or deleted cell
   while(((hm->hash_array)[hashIndex]) != NULL && ((hm->hash_array)[hashIndex])->key != 0) {
      //go to next cell
      ++hashIndex;

      //wrap around the table
      hashIndex %= hm->arr_size;
   }
   printf("Deep into insert\n");
   fflush(stdout);
   ((hm->hash_array)[hashIndex]) = item;
   (hm->num_elem)+= 1;
   if(hm->num_elem*2.0 > hm->arr_size){
     rehash(hm, 1);
   }
   printf("End of insert\n");
   fflush(stdout);
}

DataItem* delete(hashmap* hm,  unsigned int    key) {

   //get the hash
   int hashIndex = (hm->hashcode)((hm->arr_size), key);

   //move in array until an empty
   while(((hm->hash_array)[hashIndex]) != NULL) {

      if(((hm->hash_array)[hashIndex])->key == key) {
         DataItem* temp = ((hm->hash_array)[hashIndex]);

         //assign a dummy item at deleted position
         (hm->hash_array)[hashIndex] = dummyItem;
         (hm->num_elem)-= 1;
         if(hm->num_elem*8.0 < hm->arr_size && hm->arr_size > DEFAULT_SIZE){
           rehash(hm, 0);
         }
         return temp;
      }

      //go to next cell
      ++hashIndex;

      //wrap around the table
      hashIndex = hashIndex %hm->arr_size;
   }
   return NULL;
}

void rehash(hashmap* hm, int dir){
  float size_change = 0.5;
  if(dir == 1){
    size_change = 2;
  }
  int new_arr_size = ((int) (hm->arr_size * size_change));
  DataItem** new_hash_array = calloc((new_arr_size),sizeof(DataItem*));
  DataItem *item;
  int index;
  for(int i =0; i< hm->arr_size; i++){
    item = hm->hash_array[i];
    if(item != NULL && item->key != 0){
      index = (hm->hashcode)(new_arr_size, item->key);
      while(new_hash_array[index] != NULL) {
         ++index;
         index %= new_arr_size;
      }
      new_hash_array[index] = item;
    }
  }
  free(hm->hash_array);
  hm->arr_size = new_arr_size;
  hm->hash_array = new_hash_array;
}

void display(hashmap* hm) {
   int i = 0;

   for(i = 0; i<hm->arr_size; i++) {

      if(hm->hash_array[i] != NULL)
          if((hm->hash_array[i]->data == NULL)){
            printf(" (%u)",hm->hash_array[i]->key);
          }else{
            printf(" (%u,%u)",hm->hash_array[i]->key,*(unsigned int *)(hm->hash_array[i]->data));
          }

      else
         printf(" ~~ ");
   }
   printf("\n");
}

// int main() {
//    dummyItem = (DataItem*) malloc(sizeof(DataItem));
//    dummyItem->data = 0;
//    dummyItem->key = 0;
//    hashmap* hm = declare_map(&hashCode);
//
//    insert(hm, 1, 20);
//    insert(hm, 2, 70);
//    insert(hm, 42, 80);
//    insert(hm, 4, 25);
//    insert(hm, 12, 44);
//    insert(hm, 14, 32);
//    insert(hm, 17, 11);
//    insert(hm, 13, 78);
//    insert(hm, 37, 97);
//
//    display(hm);
//    item = search(hm, 37);
//
//    if(item != NULL) {
//       printf("Element found: %d\n", item->data);
//    } else {
//       printf("Element not found\n");
//    }
//
//    delete(hm, item->key);
//    item = search(hm, 37);
//
//    if(item != NULL) {
//       printf("Element found: %d\n", item->data);
//    } else {
//       printf("Element not found\n");
//    }
//    printf("Element found: %d\n", first(hm)->data);
// }
