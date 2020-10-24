#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

#define TABLE_LEN 1024

typedef struct Node {
    struct Node* _next;
    char*  _key;
    char* _value;
    int _len_key;
    int _len_value;
    
    Node() :
        _key(nullptr), _len_key(0), 
        _value(nullptr), _len_value(0),
        _next(nullptr) {};

} Node;

typedef struct HashTable {
    node* _table;
    size_t _table_size;
    
    HashTable(size_t size=TABLE_LEN) : _table(nullptr), _table_size(size) {
        _table = new node[_table_size]();
        for (size_t i = 0; i < _table_size; i++){
            _table[i] = new Node();
        }
    };
} HashTable;


int main(){
    auto table = HashTable(100);

}