#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

#define TABLE_LEN 1024

// Node struct for linked list
typedef struct Node {
  struct Node* next;
  char*  key;
  char* value;

  Node() :
    next(nullptr), key(nullptr), value(nullptr){};
} Node;

// Hash Table setup, array of pointers
typedef struct TableManager {
  Node** table;
  size_t table_size;

  TableManager(size_t table_size=TABLE_LEN) : table_size(table_size), table(nullptr) {
    table = new Node*[table_size];
    for (size_t i = 0; i < table_size; ++i) {
      table[i] = new Node();
    }
  };

  // Hash methods
  uint32_t murmur_hash_32(const char* key)
  {
    uint32_t hash(3323198485ul);
    for (;*key;++key) {
      hash ^= *key;
      hash *= 0x5bd1e995;
      hash ^= hash >> 15;
    }
    return hash;
  }

  uint32_t get_hash(const char* key){
    uint32_t idx = murmur_hash_32(key);
    return idx % this->table_size;
  }
} TableManager;



int main(){
  TableManager mgr = TableManager(100);
  Node** table = mgr.table;
  printf("%s\n", table[0]->key);
}
