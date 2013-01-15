#include <stdlib.h>

#include "trie.h"

Trie* trie_new()
{
  Trie* temp = malloc (sizeof(Trie));
  temp->root = NULL;
  return temp;
}

void trie_delete(Trie* t)
{
  trie_element_delete(t->root);
  free(t);
}

void trie_element_delete(TrieElement* te)
{
  if (te != NULL)
  {
    trie_element_delete(te->leftchild);
    trie_element_delete(te->rightsibbling);
    free(te);
  }
}

TrieElement* trie_element_new()
{
  TrieElement* temp = malloc(sizeof(TrieElement));
  temp->leftchild = NULL;
  temp->rightsibbling = NULL;
  temp->parent = NULL;
  return temp;
}

TrieElement* add_new_trieelement(TrieElement* parent, TrieElement* sibbling, int key, int edge, int dephth)
{
  TrieElement* temp = trie_element_new();
  temp->key = key;
  temp->edge = edge;
  temp->parent = parent;
  temp->dephth = dephth;
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
