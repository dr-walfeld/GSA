#include <stdlib.h>
#include <stdio.h>

#include "linkedlist.h"

// create new linked list
linkedlist* linked_list_new()
{
  linkedlist* temp = (linkedlist*) malloc(sizeof(linkedlist));
  temp->root = NULL;
  temp->end = NULL;

  return temp;
}

// add new item (with key)
void linked_list_add(linkedlist* l, int key)
{
  llentry* e = llentry_new(key);
  if (l->root == NULL)
  {
    l->root = e;
  }
  else
  {
    l->end->next = e;
  }
  l->end = e;
}

// create new entry
llentry* llentry_new(int key)
{
  llentry* temp = (llentry*) malloc(sizeof(llentry));
  temp->key = key;
  temp->next = NULL;

  return temp;
}

// free memory of linked list
void linked_list_delete(linkedlist* l)
{
  if (l == NULL)
    return;
  llentry* ne = l->root;
  llentry* temp;
  while (ne != NULL)
  {
    temp = ne->next;
    free (ne);
    ne = temp;
  }

  free (l);
}
