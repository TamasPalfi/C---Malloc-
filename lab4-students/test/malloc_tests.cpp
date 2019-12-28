#include <gtest/gtest.h>
#include "../src/my_malloc.h"

using namespace std;

//New Tests

TEST(TestsForStudents, BasicFunctionality)
{
  //makes sure the universe isn't unraveling
  node_t* nd = heap();
  ASSERT_TRUE(nd != NULL) << "Failure in heap test BasicFunctionality\n"
  << "The original heap has no free list\n"
  << "Something has gone horribly wrong in the template code\n"
  << "You probably want to consult a TA or the professor";
  ASSERT_TRUE(nd->size == 4080) << "Failure in heap test BasicFunctionality\n"
  << "The original heap does not have the proper size\n"
  << "Something has gone horribly wrong in the template code\n"
  << "You probably want to consult a TA or the professor";
  ASSERT_TRUE(nd->next == NULL) << "Failure in heap test BasicFunctionality\n"
  << "The original heap has more than one node in the free list\n"
  << "Something has gone horribly wrong in the template code\n"
  << "You probably want to consult a TA or the professor";
}

TEST(TestsForStudents, MoreBasicFunctionality)
{
  //*sigh* this is actually making sure reset works
  reset_heap();
  node_t* nd = free_list();
  ASSERT_TRUE(nd != NULL) << "Failure in reset test MoreBasicFunctionality\n"
  << "The reset heap has no free list\n"
  << "Something has gone horribly wrong in the template code\n"
  << "You probably want to consult a TA or the professor";
  ASSERT_TRUE(nd->size == 4080) << "Failure in reset test MoreBasicFunctionality\n"
  << "The reset heap does not have the proper size\n"
  << "Something has gone horribly wrong in the template code\n"
  << "You probably want to consult a TA or the professor";
  ASSERT_TRUE(nd->next == NULL) << "Failure in reset test MoreBasicFunctionality\n"
  << "The reset heap has more than one node in the free list\n"
  << "Something has gone horribly wrong in the template code\n"
  << "You probably want to consult a TA or the professor";
}

TEST(TestsForStudents, FindFree_InNewHeap)
{
  //this tests that find free works in the most generic situation
  //everything is one free node
  reset_heap();
  char* c = (char*) free_list();
  node_t* found = NULL;
  node_t* previous = NULL;
  
  find_free(100, &found, &previous);
  
  ASSERT_TRUE(found != NULL) << "Failure in find_free test FindFree_InNewHeap\n"
  << "*found (\"found\" in this test) is NULL, but should have virtual address 0\n"
  << "Perhaps you are changing what found points to rather than changing the value of *found";
  ASSERT_TRUE(found == (node_t*)c) << "Failure in find_free test FindFree_InNewHeap\n"
  << "*found (\"found\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 0";
  ASSERT_TRUE(found->size == 4080) << "Failure in find_free test FindFree_InNewHeap\n"
  << "*found (\"found\" in this test) has size " << (found->size)
  << ", but should have size 4080";
  ASSERT_TRUE(found->next == NULL) << "Failure in find_free test FindFree_InNewHeap\n"
  << "*found (\"found\" in this test) has next with virtual address " << (((char*)(found->next))-c)
  << ", which should be NULL";
  
  ASSERT_TRUE(previous == NULL) << "Failure in find_free test FindFree_InNewHeap\n"
  << "*previous (\"previous\" in this test) has virtual address " << (((char*)previous)-c)
  << ", but should be NULL\n"
  << "Perhaps you are initialising *previous to something";
}

TEST(TestsForStudents, FindFree_LargestInNewHeap)
{
  //this tests that find free works for largest possible size
  //everything is one free node
  reset_heap();
  char* c = (char*) free_list();
  node_t* found = NULL;
  node_t* previous = NULL;
  
  find_free(4064, &found, &previous);
  
  ASSERT_TRUE(found != NULL) << "Failure in find_free test FindFree_LargestInNewHeap\n"
  << "*found (\"found\" in this test) is NULL, but should have virtual address 0\n"
  << "Perhaps you are changing what found points to rather than changing the value of *found\n"
  << "Perhaps you are checking for more free space than you need (need size + sizeof(header_t) free bytes)";
  ASSERT_TRUE(found == (node_t*)c) << "Failure in find_free test FindFree_LargestInNewHeap\n"
  << "*found (\"found\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 0";
  ASSERT_TRUE(found->size == 4080) << "Failure in find_free test FindFree_LargestInNewHeap\n"
  << "*found (\"found\" in this test) has size " << (found->size)
  << ", but should have size 4080";
  ASSERT_TRUE(found->next == NULL) << "Failure in find_free test FindFree_LargestInNewHeap\n"
  << "*found (\"found\" in this test) has next with virtual address " << (((char*)(found->next))-c)
  << ", which should be NULL";
  
  ASSERT_TRUE(previous == NULL) << "Failure in find_free test FindFree_LargestInNewHeap\n"
  << "*previous (\"previous\" in this test) has virtual address " << (((char*)previous)-c)
  << ", but should be NULL\n"
  << "Perhaps you are initialising *previous to something";
}

TEST(TestsForStudents, FindFree_TooLargeInNewHeap)
{
  //this tests that find free works for overly large sizes possible size
  //everything is one free node
  reset_heap();
  char* c = (char*) free_list();
  node_t *found = NULL;
  node_t *previous = NULL;
  
  find_free(4065, &found, &previous);
  
  ASSERT_TRUE(found == NULL) << "Failure in find_free test FindFree_TooLargeInNewHeap\n"
  << "*found (\"found\" in this test) has virtual address " << (((char*)found)-c)
  << ", but should be NULL\n"
  << "Perhaps you are checking for less free space than you need (need size + sizeof(header_t) free bytes)";
  //previous is null or not doesn't matter to me at this point
}

TEST(TestsForStudents, FindFree_SmallInUsedHeap)
{
  //
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+2032);
  header_t* h1 = (header_t*)(c+1016);
  n1->size = 1000;
  n2->size = 2048;
  h1->size = 1000;
  n1->next = n2;
  n2->next = NULL;
  h1->magic = MAGIC;
  node_t* found = NULL;
  node_t* previous = NULL;
  
  find_free(500, &found, &previous);
  
  ASSERT_TRUE(found != NULL) << "Failure in find_free test FindFree_SmallInUsedHeap\n"
  << "*found (\"found\" in this test) is NULL, but should have virtual address 0\n"
  << "Perhaps you are changing what found points to rather than changing the value of *found";
  ASSERT_TRUE(found == n1) << "Failure in find_free test FindFree_SmallInUsedHeap\n"
  << "*found (\"found\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 0\n"
  << "Perhaps you are changing what found points to rather than changing the value of *found";
  
  ASSERT_TRUE(previous == NULL) << "Failure in find_free test FindFree_SmallInUsedHeap\n"
  << "*previous (\"previous\" in this test) has virtual address " << (((char*)previous)-c)
  << ", but should be NULL\n"
  << "Perhaps you are initialising *previous to something";
}

TEST(TestsForStudents, FindFree_LargestInUsedHeap)
{
  reset_heap();
  char* c = (char*) free_list(); //let the madness begin
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+2032);
  header_t* h1 = (header_t*)(c+1016);
  n1->size = 1000;
  n2->size = 2048;
  h1->size = 1000;
  n1->next = n2;
  n2->next = NULL;
  h1->magic = MAGIC;
  node_t *found = NULL;
  node_t *previous = NULL;
  
  find_free(2032, &found, &previous);
  
  ASSERT_TRUE(found != NULL) << "Failure in find_free test FindFree_LargestInUsedHeap\n"
  << "*found (\"found\" in this test) is NULL, but should have virtual address 2032\n"
  << "Perhaps you are looking for more free space than you need (need size + sizeof(header_t) free bytes)";
  ASSERT_TRUE(found == n2) << "Failure in find_free test FindFree_LargestInUsedHeap\n"
  << "*found (\"found\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 2032\n"
  << "Perhaps you are looking for more free space than you need (need size + sizeof(header_t) free bytes)";
  
  ASSERT_TRUE(previous == n1) << "Failure in find_free test FindFree_LargestInUsedHeap\n"
  << "*previous (\"previous\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 0\n"
  << "Perhaps you are changing what previous points to rather than changing the value of *previous";
}

TEST(TestsForStudents, FindFree_TooLargeInUsedHeap)
{
  reset_heap();
  char* c = (char*) free_list(); //let the madness begin
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+2032);
  header_t* h1 = (header_t*)(c+1016);
  n1->size = 1000;
  n2->size = 2048;
  h1->size = 1000;
  n1->next = n2;
  n2->next = NULL;
  h1->magic = MAGIC;
  node_t* found = NULL;
  node_t* previous = NULL;
  
  find_free(2033, &found, &previous);
  
  ASSERT_TRUE(found == NULL) << "Failure in find_free test FindFree_TooLargeInUsedHeap\n"
  << "*found (\"found\" in this test) has virtual address " << (((char*)found)-c)
  << ", but should be NULL\n"
  << "Perhaps you are looking for less free space than you need (need size + sizeof(header_t) free bytes)";
  //behavior for previous unspecified
}

TEST(TestsForStudents, Split_NewHeap)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* found = NULL;
  node_t* previous = NULL;
  header_t* allocated = NULL;
  
  //hardcoding find_free(100,found,previous)
  found = (node_t*)c;
  
  split(100,&previous,&found,&allocated);
  node_t* hd = free_list();
  
  ASSERT_TRUE(allocated != NULL) << "Failure in split test Split_NewHeap\n"
  << "*allocated (\"allocated\" in this test) in NULL, but should have virtual address 0";
  ASSERT_TRUE(allocated == (header_t*)c) << "Failure in split test Split_NewHeap\n"
  << "*allocated (\"allocated\" in this test) has virtual address " << (((char*)allocated)-c)
  << ", but should have virtual address 0";
  ASSERT_TRUE(allocated->size == 100) << "Failure in split test Split_NewHeap\n"
  << "*allocated (\"allocated\" in this test) has size " << (allocated->size)
  << ", but should have size 100\n"
  << "Perhaps you forgot to set the size for the allocated node\n"
  << "Perhaps you set it to size + sizeof(header_t)";
  ASSERT_TRUE(allocated->magic == MAGIC) << "Failure in split test Split_NewHeap\n"
  << "*allocated (\"allocated\" in this test) doesn't have the magic value.\n"
  << "Perhaps you forgot to set the magic value for the allocated node";
  
  ASSERT_TRUE(found != NULL) << "Failure in split test Split_NewHeap\n"
  << "*free_block (\"found\" in this test) is NULL, but should have virtual address 116";
  ASSERT_TRUE(found == (node_t*)(c+116)) << "Failure in split test Split_NewHeap\n"
  << "*free_block (\"found\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 116";
  ASSERT_TRUE(found->size == 3964) << "Failure in split test Split_NewHeap\n"
  << "*free_block (\"found\" in this test) has size " << (found->size)
  << ", but should have size 3964\n"
  << "Perhaps you forgot to set the size for the new free node\n"
  << "Perhaps you didn't decrease size by sizeof(header_t) (if you're getting size = 3980)\n"
  << "Perhaps you decreases size by sizeof(header_t) and sizeof(node_t) (if you're getting size = 3948)";
  ASSERT_TRUE(found->next == NULL) << "Failure in split test Split_NewHeap\n"
  << "*free_block (\"found\" in this test) has next with virtual address " << (found->next)
  << ", which should be NULL\n"
  << "Perhaps you forgot to set the next for the new free node";
  
  ASSERT_TRUE(previous == NULL) << "Failure in split test Split_NewHeap\n"
  << "*previous (\"previous\" in this test) has virtual address " << (((char*)(previous))-c)
  << ", but should be NULL\n"
  << "Perhaps you altered *previous when it was NULL";
  
  ASSERT_TRUE(hd != NULL) << "Failure in split test Split_NewHeap\n"
  << "The head of the free list is NULL, but should be *free_block (with virtual address 116)\n"
  << "Perhaps you forget to move the head when *previous was NULL";
  ASSERT_TRUE(hd == found) << "Failure in split test Split_NewHeap\n"
  << "The head of the free list has virtual address " << (((char*)(hd))-c)
  << ", but should be *free_block (with virtual address 116)\n"
  << "Perhaps you forget to move the head when *previous was NULL";
}

TEST(TestsForStudents, Split_UsedHeap)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  n1->size = 100;
  n2->size = 500;
  n3->size = 1000;
  n1->next = n2;
  n2->next = n3;
  n3->next = NULL;
  //assume allocated regions in between
  
  node_t* found = NULL;
  node_t* previous = NULL;
  header_t* allocated = NULL;
  
  //hardcoding find_free(100, &found, &previous);
  found = n2;
  previous = n1;
  
  split(100,&previous,&found,&allocated);
  
  ASSERT_TRUE(allocated != NULL) << "Failure in split test Split_UsedHeap\n"
  << "*allocated (\"allocated\" in this test) is NULL, but should have virtual address 500";
  ASSERT_TRUE(allocated == (header_t*)n2) << "Failure in split test Split_UsedHeap\n"
  << "*allocated (\"allocated\" in this test) has virtual address " << (((char*)allocated)-c)
  << ", but should have virtual address 500";
  ASSERT_TRUE(allocated->size == 100) << "Failure in split test Split_UsedHeap\n"
  << "*allocated (\"allocated\" in this test) has size " << (allocated->size)
  << ", but should have size 100\n"
  << "Perhaps you forgot to set the size for the allocated node\n"
  << "Perhaps you set it to size + sizeof(header_t)";
  ASSERT_TRUE(allocated->magic == MAGIC) << "Failure in split test Split_UsedHeap\n"
  << "*allocated (\"allocated\" in this test) doesn't have the magic value.\n"
  << "Perhaps you forgot to set the magic value for the allocated node";
  
  ASSERT_TRUE(found != NULL) << "Failure in split test Split_UsedHeap\n"
  << "*free_block (\"found\" in this test) is NULL, but should have virtual address 616";
  ASSERT_TRUE(found == (node_t*)(c+616)) << "Failure in split test Split_UsedHeap\n"
  << "*free_block (\"found\" in this test) has virtual address " << (((char*)(found))-c)
  << ", but should have virtual address 616";
  ASSERT_TRUE(found->size == 384) << "Failure in split test Split_UsedHeap\n"
  << "*free_block (\"found\" in this test) has size " << (found->size)
  << ", but should have size 384\n"
  << "Perhaps you forgot to set the size for the new free node\n"
  << "Perhaps you didn't decrease size by sizeof(header_t) (if you're getting size = 400)\n"
  << "Perhaps you decreases size by sizeof(header_t) and sizeof(node_t) (if you're getting size = 368)";
  ASSERT_TRUE(found->next != NULL) << "Failure in split test Split_UsedHeap\n"
  << "*free_block (\"found\" in this test) has next = NULL, which should have virtual address 1000\n"
  << "Perhaps you forgot to set the next for the new free node";
  ASSERT_TRUE(found->next == n3) << "Failure in split test Split_UsedHeap\n"
  << "*free_block (\"found\" in this test) has next with virtual address "
  << (((char*)(found->next))-c)
  << ", which should have virtual address 1000\n"
  << "Perhaps you forgot to set the next for the new free node";
  
  ASSERT_TRUE(previous != NULL) << "Failure in split test Split_UsedHeap\n"
  << "*previous (\"previous\" in this test) is NULL, but should have virtual address 100";
  ASSERT_TRUE(previous == n1) << "Failure in split test Split_UsedHeap\n"
  << "*previous (\"previous\" in this test) has virtual address " << (((char*)(previous))-c)
  << ", but should have virtual address 100";
  ASSERT_TRUE(previous->size == 100) << "Failure in split test Split_UsedHeap\n"
  << "*previous (\"previous\" in this test) has size " << (previous->size)
  << ", but should have size 100\n";
  ASSERT_TRUE(previous->next != NULL) << "Failure in split test Split_UsedHeap\n"
  << "*previous (\"previous\" in this test) has next = NULL, which should have virtual address 616";
  ASSERT_TRUE(previous->next == found) << "Failure in split test Split_UsedHeap\n"
  << "*previous (\"previous\" in this test) has next with virtual address "
  << (((char*)(previous->next))-c)
  << ", which should have virtual address 616\n"
  << "Perhaps you forgot to change previous->next when it isn't null (if you're getting previous->next = 500)";
}

TEST(TestsForStudents, MyMalloc_NewHeap)
{
  reset_heap();
  char* c = (char*) free_list();
  void* addr = my_malloc(4064);
  
  ASSERT_TRUE(addr != NULL) << "Failure in my_malloc test MyMalloc_NewHeap\n"
  << "Allocated address (\"addr\" in this test) is NULL, but should have virtual address 16";
  ASSERT_TRUE(addr == (void*)(c+16)) << "Failure in my_malloc test MyMalloc_NewHeap\n"
  << "Returned pointer (\"addr\" in this test) has virtual address " << (((char*)addr)-c)
  << ", but should have virtual address 16\n"
  << "Perhaps you didn't return the pointer to start of empty region (if virtual address is 0)";
  ASSERT_TRUE(*((unsigned int*)(c+8)) == MAGIC) << "Failure in my_malloc test MyMalloc_NewHeap\n"
  << "Returned pointer (\"addr\" in this test) doesn't have a corresponding magic value";
  ASSERT_TRUE(*((unsigned int*)c) == 4064) << "Failure in my_malloc test MyMalloc_NewHeap\n"
  << "Returned pointer (\"addr\" in this test) has corresponding size " << (*((unsigned int*)c))
  << ", but should have corresponding size 4064";
}

TEST(TestsForStudents, MyMalloc_NewHeapTooLarge)
{
  reset_heap();
  char* c = (char*) free_list();
  void* addr = my_malloc(4065);
  
  ASSERT_TRUE(addr == NULL) << "Failure in my_malloc test MyMalloc_NewHeap\n"
  << "Returned pointer (\"addr\" in this test) has virtual address " << (((char*)addr)-c)
  << ", but should be NULL\n"
  << "Perhaps you aren't returning NULL when find_free can't find any free nodes";
}

TEST(TestsForStudents, MyMalloc_UsedHeap)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  n1->size = 100;
  n2->size = 500;
  n3->size = 1000;
  n1->next = n2;
  n2->next = n3;
  n3->next = NULL;
  //assume allocated regions in between
  
  void* addr = my_malloc(500);
  
  ASSERT_TRUE(addr != NULL) << "Failure in my_malloc test MyMalloc_UsedHeap\n"
  << "Allocated address (\"addr\" in this test) is NULL, but should have virtual address 1016";
  ASSERT_TRUE(addr == (void*)(c+1016)) << "Failure in my_malloc test MyMalloc_UsedHeap\n"
  << "Returned pointer (\"addr\" in this test) has virtual address " << (((char*)addr)-c)
  << ", but should have virtual address 1016\n"
  << "Perhaps you didn't return the pointer to start of empty region (if virtual address is 1000)";
  ASSERT_TRUE(*((unsigned int*)(c+1008)) == MAGIC) << "Failure in my_malloc test MyMalloc_UsedHeap\n"
  << "Returned pointer (\"addr\" in this test) doesn't have a corresponding magic value";
  ASSERT_TRUE(*((unsigned int*)(c+1000)) == 500) << "Failure in my_malloc test MyMalloc_UsedHeap\n"
  << "Returned pointer (\"addr\" in this test) has corresponding size " << (*((unsigned int*)(c+1000)))
  << ", but should have corresponding size 500";
}

TEST(TestsForStudents, Coalesce_node5_in_list54321)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  node_t* n4 = (node_t*)(c+1500);
  node_t* n5 = (node_t*)(c+2000);
  n1->size = 484;
  n2->size = 484;
  n3->size = 484;
  n4->size = 484;
  n5->size = 2080;
  n1->next = NULL;
  n2->next = n1;
  n3->next = n2;
  n4->next = n3;
  n5->next = n4;
  
  coalesce(n5);
  
  ASSERT_TRUE(n5->size == 2080) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 5's size is " << (n5->size)
  << ", but should be 2080\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n5->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 5 has next = NULL, which should have virtual address 1500\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n5->next == n4) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "node 5 has next with virtual address " << (((char*)(n5->next))-c)
  << ", which should be virtual address 1500\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  
  ASSERT_TRUE(n4->size == 484) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 4's size is " << (n4->size)
  << ", but should be 484\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n4->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 4 has next = NULL, which should have virtual address 1000\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n4->next == n3) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 4 has next with virtual address " << (((char*)(n4->next))-c)
  << ", which should be virtual address 1000\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  
  ASSERT_TRUE(n3->size == 484) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 3's size is " << (n3->size)
  << ", but should be 484\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n3->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 3 has next = NULL, which should have virtual address 500\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n3->next == n2) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 3 has next with virtual address " << (((char*)(n3->next))-c)
  << ", which should be virtual address 500\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  
  ASSERT_TRUE(n2->size == 484) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 2's size is " << (n2->size)
  << ", but should be 484\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n2->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 2 has next = NULL, which should have virtual address 0\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n2->next == n1) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 2 has next with virtual address " << (((char*)(n5->next))-c)
  << ", which should be virtual address 0\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  
  ASSERT_TRUE(n1->size == 484) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 1's size is " << (n1->size)
  << ", but should be 484\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
  ASSERT_TRUE(n1->next == NULL) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should be NULL\n"
  << "Perhaps you are coalescing nodes physically before free_block with free_block";
}

TEST(TestsForStudents, Coalesce_node5_in_list12345)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  node_t* n4 = (node_t*)(c+1500);
  node_t* n5 = (node_t*)(c+2000);
  n1->size = 484;
  n2->size = 484;
  n3->size = 484;
  n4->size = 484;
  n5->size = 2080;
  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = NULL;
  
  coalesce(n5);
  
  ASSERT_TRUE(n1->size == 484) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 1's size is " << (n1->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n1->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 1 has next = NULL, which should have virtual address 500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n1->next == n2) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should have virtual address 500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n2->size == 484) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 2's size is " << (n2->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n2->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 2 has next = NULL, which should have virtual address 1000\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n2->next == n3) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 2 has next with virtual address " << (((char*)(n2->next))-c)
  << ", which should have virtual address 1000\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n3->size == 484) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 3's size is " << (n3->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n3->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 3 has next = NULL, which should have virtual address 1500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n3->next == n4) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 3 has next with virtual address " << (((char*)(n3->next))-c)
  << ", which should have virtual address 1500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n4->size == 484) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 4's size is " << (n4->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n4->next != NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 4 has next = NULL, which should have virtual address 2000\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n4->next == n5) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 4 has next with virtual address " << (((char*)(n4->next))-c)
  << ", which should have virtual address 2000\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n5->size == 2080) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 5's size is " << (n5->size)
  << ", but should be 2080";
  ASSERT_TRUE(n5->next == NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 5 has next with virtual address " << (((char*)(n5->next))-c)
  << ", which should be NULL";
}

TEST(TestsForStudents, Coalesce_node4_in_list12345)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  node_t* n4 = (node_t*)(c+1500);
  node_t* n5 = (node_t*)(c+2000);
  n1->size = 484;
  n2->size = 484;
  n3->size = 484;
  n4->size = 484;
  n5->size = 2080;
  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = NULL;
  
  coalesce(n4);
  
  ASSERT_TRUE(n1->size == 484) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 1's size is " << (n1->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n1->next != NULL) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 1 has next = NULL, which should have virtual address 500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n1->next == n2) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should have virtual address 500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n2->size == 484) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 2's size is " << (n2->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n2->next != NULL) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 2 has next = NULL, which should have virtual address 1000\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n2->next == n3) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 2 has next with virtual address " << (((char*)(n2->next))-c)
  << ", which should have virtual address 1000\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n3->size == 484) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 3's size is " << (n3->size)
  << ", but should be 484\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n3->next != NULL) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 3 has next = NULL, which should have virtual address 1500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  ASSERT_TRUE(n3->next == n4) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 3 has next with virtual address " << (((char*)(n3->next))-c)
  << ", which should have virtual address 1500\n"
  << "Perhaps you are affecting things in the freelist before free_block";
  
  ASSERT_TRUE(n4->size == 2580) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 4's size is " << (n4->size)
  << ", but should be 2580\n"
  << "Perhaps you aren't changing free_block->size after coalescing";
  ASSERT_TRUE(n4->next == NULL) << "Failure in coalesce test Coalesce_node4_in_list12345\n"
  << "Node 4 has next with virtual address " << (((char*)(n4->next))-c)
  << ", which should be NULL\n"
  << "Perhaps you aren't changing free_block->next after coalescing";
}

TEST(TestsForStudents, Coalesce_node1_in_list12345)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  node_t* n4 = (node_t*)(c+1500);
  node_t* n5 = (node_t*)(c+2000);
  n1->size = 484;
  n2->size = 484;
  n3->size = 484;
  n4->size = 484;
  n5->size = 2080;
  n1->next = n2;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = NULL;
  
  coalesce(n1);
  
  ASSERT_TRUE(n1->size == 4080) << "Failure in coalesce test Coalesce_node5_in_list54321\n"
  << "Node 1's size is " << (n4->size)
  << ", but should be 4080\n"
  << "Perhaps you aren't recurring on free_block after a successful coalesce";
  ASSERT_TRUE(n1->next == NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 1 has next with virtual address " << (((char*)(n4->next))-c)
  << ", which should be NULL\n"
  << "Perhaps you aren't recurring on free_block after a successful coalesce";
}

TEST(TestsForStudents, Coalesce_node2_in_list23415)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  node_t* n4 = (node_t*)(c+1500);
  node_t* n5 = (node_t*)(c+2000);
  n1->size = 484;
  n2->size = 484;
  n3->size = 484;
  n4->size = 484;
  n5->size = 2080;
  n1->next = n5;
  n2->next = n3;
  n3->next = n4;
  n4->next = n1;
  n5->next = NULL;
  
  coalesce(n2);
  
  ASSERT_TRUE(n2->size == 1484) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 2's size is " << (n2->size)
  << ", but should be 1484";
  ASSERT_TRUE(n2->next != NULL) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 2 has next = NULL, which should have virtual address 0";
  ASSERT_TRUE(n2->next == n1) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 2 has next with virtual address " << (((char*)(n2->next))-c)
  << ", which should have virtual address 0";
  
  ASSERT_TRUE(n1->size == 484) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 1's size is " << (n4->size)
  << ", but should be 484";
  ASSERT_TRUE(n1->next != NULL) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 1 has next = NULL, which should have virtual address 2000";
  ASSERT_TRUE(n1->next == n5) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should have virtual address 2000";
  
  ASSERT_TRUE(n5->size == 2080) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 5's size is " << (n5->size)
  << ", but should be 2080";
  ASSERT_TRUE(n5->next == NULL) << "Failure in coalesce test Coalesce_node2_in_list23415\n"
  << "Node 5 has next with virtual address " << (((char*)(n5->next))-c)
  << ", which should be NULL";
}

TEST(TestsForStudents, MyFree_node1_in_list2345)
{
  reset_heap();
  char* c = (char*) free_list();
  node_t* n1 = (node_t*)c;
  header_t* h1 = (header_t*) n1;
  node_t* n2 = (node_t*)(c+500);
  node_t* n3 = (node_t*)(c+1000);
  node_t* n4 = (node_t*)(c+1500);
  node_t* n5 = (node_t*)(c+2000);
  h1->size = 484;
  n2->size = 484;
  n3->size = 484;
  n4->size = 484;
  n5->size = 2080;
  h1->magic = MAGIC;
  n2->next = n3;
  n3->next = n4;
  n4->next = n5;
  n5->next = NULL;
  set_head(n2);
  
  my_free((void*)(((char*)h1)+16));
  node_t* hd = free_list();
  
  ASSERT_TRUE(n1->size == 4080) << "Failure in my_free test MyFree_node1_in_list2345\n"
  << "Node 1's size is " << (n1->size)
  << ", but should be 4080\n"
  << "Perhaps you are not setting the de-allocated region to the head of the free list\n"
  << "Perhaps you are not coalescing after freeing";
  ASSERT_TRUE(n1->next == NULL) << "Failure in coalesce test Coalesce_node5_in_list12345\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should be NULL\n"
  << "Perhaps you are not setting the de-allocated region to the head of the free list\n"
  << "Perhaps you are not coalescing after freeing";
  
  ASSERT_TRUE(hd != NULL) << "Failure in my_free test MyFree_node1_in_list2345\n"
  << "The head of the free list is NULL, but should have virtual address 0\n"
  << "Perhaps you are not setting the head of the free list to the de-allocated region";
  ASSERT_TRUE(hd == n1) << "Failure in my_free test MyFree_node1_in_list2345\n"
  << "The head of the free list has virtual address " << (((char*)hd)-c)
  << ", but should have virtual address 0\n"
  << "Perhaps you are not setting the head of the free list to the de-allocated region";
}

TEST(TestsForStudents, MyFree_FreeUp)
{
  //this test may destroy list structure, but that should be fine
  reset_heap();
  char* c = (char*) free_list();
  header_t* h1 = (header_t*)c;
  node_t* n1 = (node_t*) h1;
  header_t* h2 = (header_t*)(c+500);
  node_t* n2 = (node_t*) h2;
  header_t* h3 = (header_t*)(c+1000);
  node_t* n3 = (node_t*) h3;
  node_t* n4 = (node_t*)(c+2000);
  h1->size = 484;
  h2->size = 484;
  h3->size = 984;
  n4->size = 2080;
  h1->magic = MAGIC;
  h2->magic = MAGIC;
  h3->magic = MAGIC;
  n4->next = NULL;
  set_head(n4);
  
  my_free((void*)(((char*)h1)+16));
  my_free((void*)(((char*)h2)+16));
  my_free((void*)(((char*)h3)+16));
  node_t* hd = free_list();
  
  ASSERT_TRUE(n3->size == 984) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 3's size is " << (n3->size)
  << ", but should be 984";
  ASSERT_TRUE(n3->next != NULL) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 3 has next = NULL, which should have virtual address 500";
  ASSERT_TRUE(n3->next == n2) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 3 has next with virtual address " << (((char*)(n3->next))-c)
  << ", which should have virtual address 500";
  
  ASSERT_TRUE(n2->size == 484) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 2's size is " << (n2->size)
  << ", but should be 484";
  ASSERT_TRUE(n2->next != NULL) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 2 has next = NULL, which should have virtual address 0";
  ASSERT_TRUE(n2->next == n1) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 2 has next with virtual address " << (((char*)(n2->next))-c)
  << ", which should have virtual address 0";
  
  ASSERT_TRUE(n1->size == 484) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 1's size is " << (n1->size)
  << ", but should be 484";
  ASSERT_TRUE(n1->next != NULL) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 1 has next = NULL, which should have virtual address 2000";
  ASSERT_TRUE(n1->next == n4) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should have virtual address 2000";
  
  ASSERT_TRUE(n4->size == 2080) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 4's size is " << (n4->size)
  << ", but should be 2080";
  ASSERT_TRUE(n4->next == NULL) << "Failure in my_free test MyFree_FreeUp\n"
  << "Node 4 has next with virtual address " << (((char*)(n4->next))-c)
  << ", which should be NULL";
  
  ASSERT_TRUE(hd != NULL) << "Failure in my_free test MyFree_FreeUp\n"
  << "The head of the free list is NULL, but should have virtual address 1000\n"
  << "Perhaps you are not setting the head of the free list to the de-allocated region";
  ASSERT_TRUE(hd == n3) << "Failure in my_free test MyFree_FreeUp\n"
  << "The head of the free list has virtual address " << (((char*)hd)-c)
  << ", but should have virtual address 1000\n"
  << "Perhaps you are not setting the head of the free list to the de-allocated region";
}

TEST(TestsForStudents, MyFree_FreeDown)
{
  //this test may destroy list structure, but that should be fine
  reset_heap();
  char* c = (char*) free_list();
  header_t* h1 = (header_t*)c;
  node_t* n1 = (node_t*) h1;
  header_t* h2 = (header_t*)(c+500);
  header_t* h3 = (header_t*)(c+1000);
  node_t* n4 = (node_t*)(c+2000);
  h1->size = 484;
  h2->size = 484;
  h3->size = 984;
  n4->size = 2080;
  h1->magic = MAGIC;
  h2->magic = MAGIC;
  h3->magic = MAGIC;
  n4->next = NULL;
  set_head(n4);
  
  my_free((void*)(((char*)h3)+16));
  my_free((void*)(((char*)h2)+16));
  my_free((void*)(((char*)h1)+16));
  node_t* hd = free_list();
  
  ASSERT_TRUE(n1->size == 4080) << "Failure in my_free test MyFree_FreeDown\n"
  << "Node 1's size is " << (n1->size)
  << ", but should be 4080\n"
  << "Perhaps you are not setting the de-allocated region to the head of the free list\n"
  << "Perhaps you are not coalescing after freeing";
  ASSERT_TRUE(n1->next == NULL) << "Failure in my_free test MyFree_FreeDown\n"
  << "Node 1 has next with virtual address " << (((char*)(n1->next))-c)
  << ", which should be NULL\n"
  << "Perhaps you are not setting the de-allocated region to the head of the free list\n"
  << "Perhaps you are not coalescing after freeing";
  
  ASSERT_TRUE(hd != NULL) << "Failure in my_free test MyFree_FreeDown\n"
  << "The head of the free list is NULL, but should have virtual address 0\n"
  << "Perhaps you are not setting the head of the free list to the de-allocated region";
  ASSERT_TRUE(hd == n1) << "Failure in my_free test MyFree_FreeDown\n"
  << "The head of the free list has virtual address " << (((char*)hd)-c)
  << ", but should have virtual address 0\n"
  << "Perhaps you are not setting the head of the free list to the de-allocated region";
}

// PUBLIC TESTS


TEST(MallocTest, InitAndCheckAvailableMemory) {
  reset_heap();
  size_t size = available_memory();
  ASSERT_EQ(size, HEAP_SIZE - sizeof(node_t));
}

TEST(MallocTest, CheckNumberOfFreeNodes) {
  reset_heap();
  int count = number_of_free_nodes();
  ASSERT_EQ(count, 1);
}

TEST(MallocTest, SimpleMallocCall) {
  reset_heap();
  void *p = my_malloc(100);
  ASSERT_TRUE(p != NULL);
}

TEST(MallocTest, SimpleMallocCallNumberOfFreeNodes) {
  reset_heap();
  void *p = my_malloc(100);
  ASSERT_TRUE(p != NULL);
  int count = number_of_free_nodes();
  ASSERT_EQ(count, 1);
}

TEST(MallocTest, MallocTooMuchMemory) {
  reset_heap();
  void *p = my_malloc(10000);
  ASSERT_TRUE(p == NULL);
}

TEST(MallocTest, SimpleMallocCallCheckAvailableMemory) {
  reset_heap();
  void *p = my_malloc(100);
  ASSERT_TRUE(p != NULL);
  size_t size = available_memory();
  ASSERT_EQ(size, HEAP_SIZE - sizeof(node_t) * 2 - 100);
}

TEST(MallocTest, CheckYourHead) {
  reset_heap();
  void *p = my_malloc(100);
  ASSERT_FALSE(p == NULL);
  
  header_t *head = (header_t *)((char *)p - sizeof(header_t));
  ASSERT_EQ(head->magic, MAGIC);
}

TEST(MallocTest, SimpleFreeCheck) {
  reset_heap();
  void *p = my_malloc(100);
  ASSERT_FALSE(p == NULL);
  
  void *k = my_malloc(100);
  ASSERT_FALSE(k == NULL);
  my_free(p);
  ASSERT_EQ(number_of_free_nodes(), 2);
  k = NULL;  // to stop compiler warnings.
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
