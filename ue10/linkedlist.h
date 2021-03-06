#ifndef LINKEDLIST
#define LINKEDLIST

typedef struct entstruct {
  struct entstruct* next;
  int key;
} llentry;

typedef struct llStruct {
  llentry* root;
  llentry* end;
} linkedlist;

linkedlist* linked_list_new();
void linked_list_add(linkedlist*,int);
void linked_list_delete(linkedlist*);
llentry* llentry_new (int);
#endif
