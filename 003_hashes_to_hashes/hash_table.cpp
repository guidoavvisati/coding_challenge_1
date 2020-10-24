#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

#define TABLE_LEN 1024

// Node struct for linked list
typedef struct Node {
  struct Node *next;
  char *key;
  char *value;

  // Initialize with NULL
  Node() :
    next(nullptr), key(nullptr), value(nullptr) {};

  // Initialize memory, pass refs, copy values
  Node(const char *_key, const char *_value, Node *_next) :
    next(_next), key(nullptr), value(nullptr) {
    key = new char[strlen(_key)];
    value = new char[strlen(_value)];
    strcpy(key, _key);
    strcpy(value, _value);
  };
  

} Node;

// Hash Table setup, array of references
typedef struct TableManager {
  Node **table;
  size_t table_size;
  uint32_t table_entry_counts;

  TableManager(size_t table_size=TABLE_LEN) :
    table_size(table_size), table(nullptr), table_entry_counts(0) {
    table = new Node * [table_size]();
  };

  // Hash methods
  uint32_t murmur_hash_32(const char *key)
  {
    uint32_t hash(3323198485ul);
    for (;*key;++key) {
      hash ^= *key;
      hash *= 0x5bd1e995;
      hash ^= hash >> 15;
    }
    return hash;
  }

  uint32_t get_hash(const char *key){
    uint32_t idx = murmur_hash_32(key);
    return idx % this->table_size;
  }

  // API
  bool add(const char *key, const char *value){
    table_entry_counts++;
    uint32_t idx = get_hash(key);

    if(table[idx] == nullptr){ // Case no collisions
      table[idx] = new Node(key, value, nullptr);
    }
    else{ // Case with collision, prepend new to list
      table[idx] = new Node(key, value, table[idx]);
    }

    return true;
  }

  Node *find(const char *key){
    
    return NULL;
  }

  bool remove(Node *node){
    return 1;
  }

  
} TableManager;



int main(int argc, char *argv[]){
  
  TableManager mgr = TableManager(100);
  Node** table = mgr.table;
  if(argc > 1) printf("%d\n", mgr.get_hash(argv[1]));
  mgr.add("123", "ab");
  mgr.add("123", "mini");
  printf("%d\n", mgr.table_entry_counts);
}
