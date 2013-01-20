#ifndef TRIE
#define TRIE

/* representation of Trie node
   as left child - right sibbling tree
*/
typedef struct te {
  struct te* parent; /* parent node */
  struct te* leftchild; /* left child node */
  struct te* rightsibbling; /* right sibbling node */
  int edge; /* value of incoming edge */
  int key; /* key of node */
  int dephth; /* dephth of node in Trie */
} TrieElement;

/* Trie structure */
typedef struct {
  TrieElement* root; /* root node */
} Trie;

/* create new Trie */
Trie* trie_new();
/* add new element to with (parent - maybe NULL, to right sibbling - may be
   NULL, and key and edge value */
TrieElement* add_new_trieelement(TrieElement*, TrieElement*, int, int);
/* free memory of Trie */
void trie_delete(Trie*);

#endif
