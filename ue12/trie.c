#include <stdlib.h>
#include <assert.h>

#include "trie.h"

/* create new Trie */
Trie* trie_new()
{
  Trie* temp = malloc (sizeof(Trie));
  assert(temp);
  temp->root = NULL;
  return temp;
}

/* delete recursively element from Trie */
static void trie_element_delete(TrieElement* te)
{
  if (te != NULL)
  {
    trie_element_delete(te->leftchild);
    trie_element_delete(te->rightsibbling);
    free(te);
  }
}

/* delete Trie */
void trie_delete(Trie* t)
{
  if (t != NULL)
  {
    trie_element_delete(t->root);
    free(t);
  }
}

/* create new Trie element */
static TrieElement* trie_element_new()
{
  TrieElement* temp = malloc(sizeof(TrieElement));
  assert(temp);

  temp->leftchild = NULL;
  temp->rightsibbling = NULL;
  temp->parent = NULL;
  return temp;
}

/* add new Trie element to element parent and to
   element sibbling as right child (if not NULL);
   key specifies key, edge the edge label of incoming edge
*/
TrieElement* add_new_trieelement(TrieElement* parent, TrieElement* sibbling, int key, int edge)
{
  TrieElement* temp = trie_element_new();
  temp->key = key;
  temp->edge = edge;
  temp->parent = parent;

  if (parent == NULL)
  {
    temp->dephth = 0;
  }
  else
  {
    temp->dephth = parent->dephth+1;
  }

  if (sibbling != NULL)
  {
    sibbling->rightsibbling = temp;
  }
  else if (parent != NULL)
  {
    temp->rightsibbling = parent->leftchild;
    parent->leftchild = temp;
  }

  return temp;
}
