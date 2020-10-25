#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

typedef struct header_t {
  header_t *next;
  size_t size;
  char is_free;
} header_t;

header_t *first = NULL, *last = NULL;

header_t *get_free_block(header_t *first, size_t size) {
  header_t *curr = first;
  while(curr){
    if (curr->is_free && curr->size >= size)
      return curr;
    curr = curr->next;
  }
  return NULL;
}

void *custom_malloc(header_t *first, size_t size){
  size_t total_size;
  void *block;
  header_t *header;

  if (!size) return NULL;
  header = get_free_block(first, size);
  if (header) {
    header->is_free = 0;
    return (void*)(header + 1);
  }

  total_size = sizeof(header_t) + size;
  block = sbrk(total_size);
  if (block == (void*) -1) {
    return NULL;
  }

  header = block;
  header->size = size;
  header->is_free = 0;
  header->next = NULL;
  if (!first)
    first = header;
  if (last)
    last->next = header;
  last = header;
  return (void*)(header + 1);
}



