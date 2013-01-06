#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "linkedlist.h"

// pre-calculate 0..r^q
int* get_pot_list(int r, int q)
{
  int* result = (int*) malloc((q+1)*sizeof(int));
  result[0] = 1;
  int i;
  for (i = 1; i <= q; i++)
  {
    result[i] = result[i-1]*r;
  }

  return result;
}

// translate character with alphabet A
inline int translate (int c, int* A, int* fail)
{
  *fail = 0;
  if (A[c] == -1)
  {
    printf("ERROR: character %c is not defined in alphabet!\n", c);
    *fail = 1;
  }
  return A[c];
}

// calculate qgram distance
int qgdist(char* u, char* v, int q, int r, int* A)
{
  // length of sequences
  int m = strlen(u);
  int n = strlen(v);

  if (q > m || q > n)
    return -1;

  // precalculated exponentiation r^q
  int* rpot = get_pot_list(r,q);

  // reserve memory for tables
  int* tu = (int*) calloc(rpot[q], sizeof(int));
  int* tv = (int*) calloc(rpot[q], sizeof(int));

  // linked list for representation of C
  linkedlist* C = linked_list_new();

  unsigned long int i;
  int c = 0;
  int fail;

  // calculate first q word in u and add to list
  for (i = 1; i <= q; i++)
  {
    c += translate(u[i-1],A,&fail)*rpot[q-i];
  }
  if (fail)
    return -1;
  tu[c] = 1;
  linked_list_add(C,c);

  // calculate all subsequent q words in u and add to list (if not present)
  for (i = 1; i <= m-q; i++)
  {
    c = (c - translate(u[i-1],A,&fail)*rpot[q-1])*r + translate(u[i+q-1],A,&fail);
    if (fail)
      return -1;
    if (tu[c] == 0)
      linked_list_add(C,c);
    tu[c]++;
  }

  // calculate first q word in v and add to list (if not present)
  c = 0;
  for (i = 1; i <= q; i++)
  {
    c += translate(v[i-1],A,&fail)*rpot[q-i];
  }

  if (fail)
    return -1;
  tv[c] = 1;
  if (tu[c] == 0)
    linked_list_add(C,c);

  // calculate all subsequent q words in v and add to list (it not present)
  for (i = 1; i <= n-q; i++)
  {
    c = (c - translate(v[i-1],A,&fail)*rpot[q-1])*r + translate(v[i+q-1],A,&fail);
    if (fail)
      return -1;
    if (tv[c] == 0 && tu[c] == 0)
      linked_list_add(C,c);
    tv[c]++;
  }

  // iterate over list C and calculate distance between u and v

  llentry* curr = C->root;
  int result = 0;

  while (curr != NULL)
  {
    c = curr->key;
    result += abs(tu[c]-tv[c]);
    curr = curr->next;
  }

  // free memory
  linked_list_delete(C);
  free(rpot);
  free(tu);
  free(tv);

  return result;
}

// create new alphabet from cstring
int* new_alphabet(char* s)
{
  int len = strlen(s);
  // reserve memory (for all allowed characters)
  int* a = (int*) malloc(UCHAR_MAX*sizeof(int));;
  unsigned int i;
  // initialize with -1
  for (i = 0; i < UCHAR_MAX; i++)
  {
    a[i] = -1;
  }
  // save position of character in alphabet-string as code
  for (i = 0; i < len; i++)
  {
    // multiple characters in alphabet are not allowed => hamper encoding by
    // position
    if (a[(int)s[i]] != -1)
    {
      printf("ERROR: %c more than once in alphabet!\n",s[i]);
      return NULL;
    }
    else
    {
      a[(int)s[i]] = i;
    }
  }
  return a;
}

int main(int argc, char* argv[])
{
  if (argc < 5)
  {
    printf("usage: %s seq1 seq2 A q\n", argv[0]);
    return 0;
  }

  char* u = argv[1];
  char* v = argv[2];
  char* Astring= argv[3];
  int q;
  if (sscanf(argv[4], "%d", &q) != 1 || q < 1)
  {
    printf ("invalid parameter q=%s\n", argv[4]);
    return 1;
  }
  int r = strlen(Astring);
  int* A = new_alphabet(Astring);
  if (A == NULL)
    return 1;
  int cost = qgdist(u,v,q,r,A);
  printf("qgdist(u,v) = %d\n", cost);
  free(A);
  return 0;
}
