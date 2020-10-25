#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <unistd.h>

typedef struct header_t {
  header_t *next;
  size_t size;
  char is_free;
} header_t;

header_t *head = NULL, *tail = NULL;

void set_block(header_t *block, char is_free, size_t size, header_t *next){
  block->next = next;
  block->is_free = is_free;
  block->size = size;
}

header_t *get_free_block(size_t size) {
  header_t *curr = head;
  // Go over the list until block of suitable size is found
  while(curr){
    if (curr->is_free && curr->size >= size)
      return curr;
    curr = curr->next;
  }
  return nullptr;
}

void *custom_malloc(size_t size){
  // Caveat of this method: no merging of
  // contiguous freed blocks, so in the end
  // it will cause more segmentation
  size_t total_size;
  void *block;
  header_t *header;

  if (!size) return nullptr;
  header = get_free_block(size);
  if (header) {
    header->is_free = 0;
    return (void*)(header + 1);
  }

  total_size = sizeof(header_t) + size;
  block = sbrk(total_size);
  if (block == (void*) -1) {
    return nullptr;
  }

  header = (header_t *)block;
  set_block(header, 0, size, nullptr);

  // Update head and tail
  if (!head)
    head = header;
  if (tail)
    tail->next = header;
  return (void*)(header + 1);
}


int main(int argc, char *argv[]){
  int *i = (int *)custom_malloc(1);
  *i = 5;
  printf("%d\n", *i);
  char *str = (char *)custom_malloc(1);
  strncpy(str, "MALLOC", 8);
  printf("%s\n", str);
  
}

