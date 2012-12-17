#ifndef HASH
#define HASH

typedef struct hashStruct {
  int** hashtable;
  int m;
  int (*h)(int,int,int);
} hash;

hash* hashtable_new(int);
int hash_search(hash*,int);
int hash_insert(hash*,int,int);
void hashtable_delete(hash*);
#endif
