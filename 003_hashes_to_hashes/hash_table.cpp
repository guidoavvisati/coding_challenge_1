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

  bool update_value(const char *_value){
    if(value) delete[] value;
    value = new char[strlen(_value)];
    strcpy(value, _value);
    return true;
  }
  

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
  // uint32_t murmur_hash_32(const char *key)
  // {
  //   uint32_t hash(3323198485ul);
  //   for (;*key;++key) {
  //     hash ^= *key;
  //     hash *= 0x5bd1e995;
  //     hash ^= hash >> 15;
  //   }
  //   return hash;
  // }

  uint32_t murmur_hash_32(const char *key)
  {
    unsigned int hash = 0;
    int c;

    while (c = *key++)
        hash += c;

    return hash;
  }

  uint32_t get_hash(const char *key){
    uint32_t idx = murmur_hash_32(key);
    return idx % this->table_size;
  }

  Node *search_collision_list_by_key(Node *head, const char *key){
    while(head != nullptr){ // loop over list
      if(strcmp(head->key, key) == 0){ // match found, return
	return head;
      }
      head = head->next;
    }
    return nullptr; // no match found
  }


  // API
  bool add(const char *key, const char *value){
    uint32_t idx = get_hash(key);
    Node *head = table[idx];

    if(head == nullptr){ // Case no collisions
      table[idx] = new Node(key, value, nullptr);
      table_entry_counts++;
    }
    else{ // Case with collision
      Node *node_to_update = search_collision_list_by_key(head, key);
      if(node_to_update != nullptr){ // Case key in linked list, update value
	node_to_update->update_value(value);
      }
      else{ // Case new key, pre-pend to linked list
	table[idx] = new Node(key, value, head);
	table_entry_counts++;
      }
    }
    return true;
  }


  Node *find(const char *key){
    
    return NULL;
  }

  bool remove(Node *node){
    return 1;
  }

  void print_table(void){
    Node *head;
    for(size_t idx = 0; idx < table_size; ++idx){
      head = table[idx];
      if(head){
	do{
	  printf("[%lu]: (%s, %s)\n", idx, head->key, head->value);
	  head = head->next;
	} while(head != nullptr);
      }
    }
  }
  
  
} TableManager;



int main(int argc, char *argv[]){
  
  TableManager mgr = TableManager(108);
  if(argc > 1)
    for(int i = 1; i < argc; ++i)
      printf("%d\n", mgr.get_hash(argv[i]));
  // mgr.add("ab", "ab");
  // mgr.add("ba", "mini");
  // mgr.print_table();
  // printf("%d\n", mgr.table_entry_counts);

}

