#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "linkedlist.h"

/* create new linked list */
linkedlist* linked_list_new()
{
  linkedlist* temp = (linkedlist*) malloc(sizeof(linkedlist));
  assert(temp);
  temp->root = NULL;
  temp->end = NULL;

  return temp;
}

/* create new entry */
static llentry* llentry_new(int key)
{
  llentry* temp = (llentry*) malloc(sizeof(llentry));
  assert(temp);
  temp->key = key;
  temp->next = NULL;
  temp->data = NULL;

  return temp;
}

/* add new item (with key) */
void linked_list_add(linkedlist* l, int key, void* data)
{
  assert(l);
  llentry* e = llentry_new(key);
  e->data = data;
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

static void llentry_delete(llentry* l)
{
  if (l == NULL)
    return;
  free(l);
}

void* linked_list_pop(linkedlist* l)
{
  void* result;
  llentry* temp;

  assert(l);
  assert(!linked_list_is_empty(l));

  result = l->root->data;
  temp = l->root;
  l->root = l->root->next;

  /* check if end pointer was top element */
  if (temp == l->end)
    l->end = NULL;

  llentry_delete(temp);

  return result;

}

void linked_list_push(linkedlist* l, int key, void* data)
{
  llentry* new;

  assert(l);
  new = llentry_new(key);
  new->data = data;

  new->next = l->root;
  l->root = new;

  /* end pointer does not change */
}

/* free memory of linked list */
void linked_list_delete(linkedlist* l)
{
  if (l == NULL)
    return;

  while (!linked_list_is_empty(l))
  {
    linked_list_pop(l);
  }

  free (l);
}

int linked_list_is_empty(linkedlist* l)
{
  assert(l);
  return (l->root == NULL);
}

#ifdef UNIT_TEST

int test()
{
  linkedlist *liste1,*liste2;
  llentry* curr;
  int *value,i;

  liste1 = linked_list_new();
  assert(liste1);
  
  for (i = 0; i < 100; i++)
  {
    linked_list_add(liste1,i,(void*)NULL);
  }

  curr = liste1->root;

  for (i = 0; i < 100; i++, curr = curr->next)
  {
    assert(curr);
    assert(i == curr->key);
  }

  linked_list_delete(liste1);

  printf("first test passed (list functions)!\n");

  liste2 = linked_list_new();
  assert(liste2);

  for (i = 0; i < 100; i++)
  {
    value = malloc(sizeof(int));
    *value = i;
    linked_list_push(liste2,i,(void*)value);
  }

  for (i = 99; i >= 0; i--)
  {
    value = (int*)linked_list_pop(liste2);
    assert(i == (*value));
    free(value);
  }

  assert(linked_list_is_empty(liste2));

  linked_list_delete(liste2);

  printf("second test passed (stack functions)!\n");

  return 0;

}

int main()
{
  int retval;
  retval = test();

  return retval;
}
#endif
