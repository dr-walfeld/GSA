#ifndef TRIE
#define TRIE

typedef struct te {
  struct te* parent;
  struct te* leftchild;
  struct te* rightsibbling;
  int edge;
  int key;
  int dephth;
} TrieElement;

typedef struct {
  TrieElement* root;
} Trie;

Trie* trie_new();
TrieElement* trie_element_new();
TrieElement* add_new_trieelement(TrieElement*, TrieElement*, int, int, int);
void trie_delete(Trie*);
void trie_element_delete(TrieElement* te);

#endif
