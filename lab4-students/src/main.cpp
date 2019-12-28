#include "my_malloc.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main() {

  void *allocated[10];
  

  //this tests that find free works in the most generic situation
  //everything is one free node
  reset_heap();
  char* c = (char*) free_list();
  node_t* found = NULL;
  node_t* previous = NULL;
  
  find_free(4065, &found, &previous);

  /*if(found != NULL){
    cout <<"found:" << found->size;
  }
  if(previous != NULL){
    cout <<"prev" << previous->size;
    }*/
 

  
  /* printf("Available memory before: %zd.\n", available_memory());
  for (int i = 0; i < 10; i++) {
    allocated[i] = my_malloc(100);
  }
  print_free_list();
  printf("Available memory after: %zd.\n", available_memory());

  printf("Available memory before: %zd.\n", available_memory());
  for (int i = 0; i < 10; i++) {
    my_free(allocated[i]);
  }
  print_free_list();
  printf("Available memory after: %zd.\n", available_memory());

  reset_heap();

  printf("Available memory before: %zd.\n", available_memory());
  for (int i = 0; i < 10; i++) {
    allocated[i] = my_malloc(100);
  }
  print_free_list();
  printf("Available memory after: %zd.\n", available_memory());

  printf("Available memory before: %zd.\n", available_memory());
  for (int i = 9; i >= 0; i--) {
    my_free(allocated[i]);
  }
  print_free_list();
  printf("Available memory after: %zd.\n", available_memory());

  return 0;*/
}
