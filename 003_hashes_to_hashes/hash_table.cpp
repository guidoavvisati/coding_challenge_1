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
    next(nullptr),
    key(nullptr),
    value(nullptr)
  {};
  
  // Initialize memory, pass refs, copy values
  Node(const char *_key, const char *_value, Node *_next) :
    next(_next),
    key(nullptr),
    value(nullptr)
  {
    key = new char[strlen(_key)];
    value = new char[strlen(_value)];
    strcpy(key, _key);
    strcpy(value, _value);
  };

  // Free resources of single node
  ~Node()
  {
    printf("Freeing Node (%s, %s)\n", key, value);
    if(key){
      delete[] key;
      key = nullptr;
    }
    if(value){
      delete[] value;
      value = nullptr;
    }
    next = nullptr;    
  }

  // In case of same key, update value
  bool update_value(const char *_value){
    if(value) delete[] value;
    value = new char[strlen(_value)];
    strcpy(value, _value);
    return true;
  }

  // Case sensitive key search, static to serve as util
  static Node *search_list_by_key(Node *head, const char *key){
    while(head != nullptr){ // loop over list
      if(strcmp(head->key, key) == 0){ // match found, return
	return head;
      }
      head = head->next;
    }
    return nullptr; // no match found
  }

} Node;

// Hash Table setup, array of pointers.
// Could also use std::vector<Node *> container
// to have a built-in resizing mechanism
typedef struct HashTable{
  Node **table;
  size_t table_size;
  uint32_t table_entry_counts;
  uint32_t (*hash_func)(const char *key); // Pointer to hash method

  // Constructor with memory allocation
  // Default settings for table size and hash method
  HashTable(size_t table_size=TABLE_LEN, uint32_t (*hash_func)(const char *key)=&murmur_hash_32) :
    table_size(table_size),
    table(nullptr),
    table_entry_counts(0),
    hash_func(hash_func)
  {
    printf("New Hash Table created:\n");
    table = new Node * [table_size]();
  };

  // Destructor with memory deallocation
  ~HashTable(){
    printf("Freeing Hash Table:\n");
    printf("     1. Lists:\n");
    Node *entry;
    for(size_t idx = 0; idx < table_size; ++idx){
      entry = table[idx];
      if(entry){
	do {
	  Node *next = entry->next;
	  delete entry; // free node
	  entry = next;
	} while(entry != nullptr);
      }
    }
    
    printf("     2. Array\n");
    if(table){
      delete[] table;
      table = nullptr;
    }
  }

  // Utility method: print table
  void print(void){
    Node *head;
    printf("Number of total items in the table: %d\n", table_entry_counts);
    for(size_t idx = 0; idx < table_size; ++idx){
      head = table[idx];
      if(head){
	do {
	  printf("[%lu]: (%s, %s)\n", idx, head->key, head->value);
	  head = head->next;
	} while(head != nullptr);
      }
    }
  }

  // Utility method: print search match
  static void print_match(Node *match, const char *key){
    if(match){
      printf("Match found for [%s] with:\n", key);
      printf("    key: %s\n", match->key);
      printf("    val: %s\n", match->value);
    }
    else
      printf("Match not found for key: [%s]\n", key);      
  }

  // Utility method: get_hash
  uint32_t get_hash(const char *key){
    uint32_t idx = hash_func(key);
    return idx % table_size;
  }
  
  // Hash methods
  // TODO[GA]: Abstract *hash_32 methods to different class
  static uint32_t murmur_hash_32(const char *key){
    uint32_t hash(3323198485ul);
    for (;*key;++key) {
      hash ^= *key;
      hash *= 0x5bd1e995;
      hash ^= hash >> 15;
    }
    return hash;
  }

  static uint32_t bad_hash_32(const char *key){
    unsigned int hash = 0;
    int c;
    while (c = *key++)
      hash += c;
    return hash;
  }

  // API DEFINITION
  // Adds key, value pair to the hash table
  bool add(const char *key, const char *value){
    uint32_t idx = get_hash(key);
    Node *head = table[idx];

    if(head == nullptr){ // Case no collisions
      table[idx] = new Node(key, value, nullptr);
      table_entry_counts++;
      return true;
    }
    else{ // Case with collision
      Node *node_to_update = Node::search_list_by_key(head, key);
      if(node_to_update != nullptr){ // Case key in linked list, update value
	node_to_update->update_value(value);
	printf("collision, same key\n");
      }
      else{ // Case new key, pre-pend to linked list
	printf("collision, new key\n");
	table[idx] = new Node(key, value, head);
	table_entry_counts++;
      }
      return true;
    }
    return false;
  }

  // Finds node given key
  Node *find_node(const char *key) {
    uint32_t idx = get_hash(key);
    Node *match = Node::search_list_by_key(table[idx], key);
    print_match(match, key);
    return match; // Either null or match
  }

  // Finds value given key
  const char *find_value(const char *key){
    Node *match = find_node(key);
    if(match) return match->value;
    else return nullptr;
  }

  // Remove key, value pair from the table
  bool remove(const char *key){
    // Set-up table search
    uint32_t idx = get_hash(key);
    Node *prev = nullptr;
    Node *entry = table[idx];

    // Iterate list to find match and prev node.
    // Doubly linked list also possible, but more memory per node
    while (entry != nullptr && strcmp(entry->key, key) != 0) {
      prev = entry;
      entry = entry->next;
    }

    // Handle search result
    if(entry == nullptr) { // key not found
      printf("Match not found for key: [%s]\n", key);      
      return false;
    }
    else {
      if (prev == nullptr) { // Remove first of the list
	table[idx] = entry->next;
      }
      else { // Link prev node with entry->next
	prev->next = entry->next;
      }
      delete entry; // Destructor called, memory freed
      table_entry_counts--;
      return true;
    }
  }

} HashTable;



int main(int argc, char *argv[]){
  // Pass CLI argument COLLISIONS to investigate
  // the collision handling
  bool collisions = false;
  if(argc > 1){ 
    if(strcmp(argv[1], "COLLISIONS") == 0)
      collisions = true;
  }
  if(collisions){
    // Triggering collisions via a bad hashing method
    HashTable ht = HashTable(108, &HashTable::bad_hash_32);
    ht.add("ab", "ab");
    ht.add("ba", "mini");
    ht.print();
  }

  // With no COLLISIONS argument, use default settings
  // and a good hashing method
  HashTable ht = HashTable();
  ht.add("abcde", "123.6");
  ht.add("My stuff", "abcde");
  ht.add("Guido", "3572");
  ht.print();
  printf("%s\n", ht.find_value("abcde"));
  ht.find_value("ABCDE");
  Node *match = ht.find_node("My stuff");
  ht.remove("My stuff");
  ht.print();
}

