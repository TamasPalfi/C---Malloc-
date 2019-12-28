#include <iostream>  //added by me
#include "my_malloc.h"
#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

using namespace std;

// A pointer to the head of the free list.
node_t *head = NULL;

//added as part of jareds'test
void set_head(node_t * nhead){
  head = nhead;
}

// The heap function returns the head pointer to the free list. If the heap
// has not been allocated yet (head is NULL) it will use mmap to allocate
// a page of memory from the OS and initialize the first free node.
node_t *heap() {
  if (head == NULL) {
    // This allocates the heap and initializes the head node.
    head = (node_t *)mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE,
                          MAP_ANON | MAP_PRIVATE, -1, 0);
    head->size = HEAP_SIZE - sizeof(node_t);
    head->next = NULL;
  }

  return head;
}

// Reallocates the heap.
void reset_heap() {
  if (head != NULL) {
    munmap(head, HEAP_SIZE);
    head = NULL;
    heap();
  }
}

// Returns a pointer to the head of the free list.
node_t *free_list() { return head; }

// Calculates the amount of free memory available in the heap.
size_t available_memory() {
  size_t n = 0;
  node_t *p = heap();
  while (p != NULL) {
    n += p->size;
    p = p->next;
  }
  return n;
}

// Returns the number of nodes on the free list.
int number_of_free_nodes() {
  int count = 0;
  node_t *p = heap();
  while (p != NULL) {
    count++;
    p = p->next;
  }
  return count;
}

// Prints the free list. Useful for debugging purposes.
void print_free_list() {
  node_t *p = heap();
  while (p != NULL) {
    printf("Free(%zd)", p->size);
    p = p->next;
    if (p != NULL) {
      printf("->");
    }
  }
  printf("\n");
}

// Finds a node on the free list that has enough available memory to
// allocate to a calling program. This function uses the "first-fit"
// algorithm to locate a free node.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
//
// RETURNS:
// found - the node found on the free list with enough memory to allocate
// previous - the previous node to the found node
//
void find_free(size_t size, node_t **found, node_t **previous) {
  // TODO

  *found = NULL;
  *previous = NULL;
    
  //get ptr node representing the head of free list 
  cout << "beginning of find_free \n";
  node_t *h = heap();
  cout << "free_list properly set \n";
  //ctr used to track for case of node to be used is the head
  int ctr = 0;
  //loop until find an accomadating node in free list using "first-fit" algorithm
  while(h != NULL){
    cout << ctr << "\n";
    //check for case that first node in free list is applicable
    if(ctr == 0){
      cout << "enters if statement for head \n ";
      cout << "h:size" << h->size << "\n";
      //check for if node fulfills size requirement
      if((h->size) >= (size + sizeof(header_t))){  //TODO : possible error wth header_t
	cout << "found free node at head \n";
	//block works so return
	*found = h;
	cout << "found node set to block of memory good!\n";  
	//head was succesful block so no prev.
	*previous = NULL;
	//exit loop
	return;
      }
    }
    //check for all other cases
    //get next node so to be able to check & return prev node (for split)  
    node_t *prev = h;                                        ///TODO: possible error/ never init prev   
    h = h->next;
    //first check if next node exists
    if(h != NULL){
      //check for if node fulfills size requirement
      if((h->size) >= (size + sizeof(header_t))){
	//block works so return
	*found = h;
	//not head was successful so return prev
	*previous = prev;
	//exit loop
	return;
      }
    }
    //increment counter when progressing to next node 
    ctr++;
    }
}

// Splits a found free node to accommodate an allocation request.
//
// The job of this function is to take a given free_node found from
// `find_free` and split it according to the number of bytes to allocate.
// In doing so, it will adjust the size and next pointer of the `free_block`
// as well as the `previous` node to properly adjust the free list.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
// previous - the previous node to the free block
// free_block - the node on the free list to allocate from
//
// RETURNS:
// allocated - an allocated block to be returned to the calling program
//
void split(size_t size, node_t **previous, node_t **free_block,
           header_t **allocated) {
  assert(*free_block != NULL);
  // TODO

  //step 1
  //track ptr for where free_block started at
  node_t *orig = *free_block;
  
  //adjust the free block ptr by num of bytes to be allocated 
  size_t actual_size = size + sizeof(header_t); 
  *free_block = (node_t*)(((char*)*free_block) + actual_size);
	
  //update size of free_block after adjustment         
  (*free_block)->size = (orig)->size - actual_size;   
	cout << " free block - original : " << *free_block - orig << endl;  
	(*free_block)->next = (orig)->next; 
	//step 2
  //cout << *free_block - orig;

  //adjust prev ptr to point to new location of free_block
  //first check if prev is null / current node is the head
  if((*previous) == NULL){
    //no prev node - assign head to where free_block starts
    head = *free_block;
  }
  else{
    //in free node list, proceed normally
  (*previous)->next = (*free_block);
  }

  //step 3

  //overlay & embed header_t to start of original free block
  //assign allocated to orig free_block
  //header_t *alloc = new header_t();
  *allocated = (header_t *) orig;                   //TODO: incorrect casting???
  //update size
  (*allocated)->size = size;
  //assign magic number
  (*allocated)->magic = MAGIC;
}

// Returns a pointer to a region of memory having at least the request `size`
// bytes.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
//
// RETURNS:
// A void pointer to the region of allocated memory
//
void *my_malloc(size_t size) {
  // TODO
  
  //define local ptrs to pass to functions
  node_t *previous = NULL;
  node_t *found = NULL;
  header_t *allocated = NULL;

  
  cout << "At beginning \n";
  
  //call find free function
  find_free(size, &found, &previous);

  cout << "find free works \n";
  
  //check to see if found is NULL
  if(found == NULL){
    return NULL;
  }  
  //call split function
  split(size, &previous, &found, &allocated);

  cout << "split works\n";
  
  //fix pointer to alloc after header_t & return
  void *alloc = (char *) allocated + sizeof(header_t);
  return alloc;
}

// Merges adjacent nodes on the free list to reduce external fragmentation.
//
// This function will only coalesce nodes starting with `free_block`. It will
// not handle coalescing of previous nodes (we don't have previous pointers!).
//
// PARAMETERS:
// free_block - the starting node on the free list to coalesce
//
void coalesce(node_t *free_block) {
  // TODO

  cout << "At bgnning of Coaleasce \n";
  
  //iterate over the free list
  while(free_block != NULL){
    cout << "enters while loop \n";
    //check if free block is adjacent to another free block
    //get next free block to compare & store current free_block (free_block now the next free block)
    node_t *curr_free_block = free_block; 
    free_block = free_block->next;
    //set up block size var needed for check
    size_t block_size  = curr_free_block->size + sizeof(header_t);       //TODO: how does the size_t var type work???
    //compare addresses of concurrent free blocks to see if equal & can coalesce
    if((curr_free_block) + block_size == free_block){         //TODO: no cast to (char *) //TODO: Address check bad or merging!!?!?!?
      //addresses are equal can be coalesced
      //merge the free blocks to one using ptr arithmetic like LL
      curr_free_block->next = free_block->next;
      free_block->next = NULL;
      //update size of merged node and node_t
      curr_free_block->size += free_block->size;
      free_block->size = curr_free_block->size;
    }
  }
}

// Frees a given region of memory back to the free list.
//
// PARAMETERS:
// allocated - a pointer to a region of memory previously allocated by my_malloc
//
void my_free(void *allocated) {
  // TODO

  //cast parameter to char
  header_t *alloc = new header_t();
  alloc = (header_t *) allocated;
  //adjust ptr to start of actual allocated block -> skip past the header_t
  alloc = alloc + sizeof(header_t);
  //assert the correct magic field of block of memory
  if(alloc->magic == MAGIC){
    //get size of allocated block
    size_t size = alloc->size;
    //change cast of allocated to node_t type
    node_t * alloc_final = new node_t();
    alloc_final = (node_t *) alloc;
    //set size of bytes to be freed
    alloc_final->size = size;
    //link in free node to start of free list
    alloc_final->next = head;
    head = alloc_final;
    }
}
