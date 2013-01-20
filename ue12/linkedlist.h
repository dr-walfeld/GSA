/* representation of simple linked list */

#ifndef LINKEDLIST
#define LINKEDLIST

/* entry in linked list */
typedef struct entstruct {
  struct entstruct* next; /* pointer to next entry in list */
  int key; /* key of entry */
  void* data; /* pointer to optional data */
} llentry;

/* linked list */
typedef struct llStruct {
  llentry* root; /* first entry in list */
  llentry* end; /* last entry in list */
} linkedlist;

/* create new linked list */
linkedlist* linked_list_new();
/* add new element to linked list end */
void linked_list_add(linkedlist*,int,void*);
/* add new element to top of linked list */
void linked_list_push(linkedlist*,int,void*);
/* get and remove element from top of linked list */
void* linked_list_pop(linkedlist*);
/* delete linked list */
void linked_list_delete(linkedlist*);
/* check if linked list is empty */
int linked_list_is_empty(linkedlist*);
#endif
