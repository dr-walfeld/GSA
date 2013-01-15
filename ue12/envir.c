#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"
#include "scorematrix.h"
#include "stack.h"
#include "trie.h"

/* function NEW in 12.2 */
int* calclookahead(char* seq, char* alphabet, int r, int q, int k, scorematrix* sm)
{
  int* look = malloc(sizeof(int)*(q+1));
  int d,i,max,score;
  look[q] = k;
  for (d = q-1; d >= 0; d--)
  {
    max = -100000;
    for (i = 0; i < r; i++)
    {
      score = get_score(sm,seq[d],alphabet[i]);
      if (score > max)
        max = score;
    }
    look[d] = look[d+1] - max;
  }

  return look;
}

Stack* calcenv(char* seq, char* alphabet, int r, int q, int k, scorematrix* sm, TrieElement* start, int* pushes, int pruning)
{
  Stack* s = stack_new();
  Stack* environment = stack_new();
  TrieElement *curr,*parent;
  int i;
  int score;
  int* look = calclookahead(seq, alphabet, r, q, k, sm); /* NEW in 12.2 */
  
  stack_push(s,start);
  (*pushes)++;
  while(!stack_is_empty(s))
  {
    parent = (TrieElement*) stack_pop(s);
    curr = NULL;
    if (parent->dephth < q)
    {
      for (i = 0; i < r; i++)
      {
        score = get_score(sm, alphabet[i], seq[parent->dephth]);
        /* if-condition NEW in 12.2 */
        if (pruning == 0 || parent->key + score >= look[parent->dephth+1])
        {
          curr = add_new_trieelement(parent,curr,parent->key + score, i, parent->dephth+1);
          stack_push(s,(void*)curr);
          (*pushes)++;
        }
      }
    }
    else if (parent->key >= k)
    {
      stack_push(environment, (void*) parent);
    }

  }

  stack_delete(s);

  return environment;
}

void show_environment(Stack* s, char* alphabet, int q, int i)
{
  TrieElement* elem;
  char toprint[q+1];
  toprint[q] = '\0';
  while(!stack_is_empty(s))
  {
    elem = (TrieElement*) stack_pop(s);
    while(elem->parent != NULL)
    {
      toprint[elem->dephth-1] = alphabet[elem->edge];
      elem = elem->parent;
    }
    printf("(%s,%d)\n", toprint, i);
  }

}

void env(char* w, char* alphabet, int q, int k, scorematrix* sm, int pruning)
{
  int m = strlen(w);
  int r = strlen(alphabet);
  Stack* environment;

  int pushes = 0;

  int i;
  for(i = 0; i < m-q+1; i++)
  {
    Trie* t = trie_new();
    TrieElement* start = add_new_trieelement(NULL,NULL,0,0,0);
    t->root = start;

    //printf("Stage 1...\n");
    environment = calcenv(w+i,alphabet,r,q,k,sm,start,&pushes, pruning);
    //printf("Stage 2...\n");
    show_environment(environment,alphabet,q,i);
    stack_delete(environment);

    trie_delete(t);
  }
  printf("%d Pushes to Stack!\n", pushes);

}

int main(int argc, char* argv[])
{
  if (argc < 7)
  {
    printf("usage: %s scorematrix alphabet w q k pruning\n", argv[0]);
    return 0;
  }

  scorematrix* sm = read_scorematrix(argv[1],0);
  if (sm == NULL)
    return 1;

  char* alphabet = argv[2];

  char* w = argv[3];

  int q,k,pruning;
  if (sscanf(argv[4],"%d",&q) != 1 || q <= 0)
  {
    printf("%s is no valid word length!\n", argv[4]);
    return 1;
  }

  if (sscanf(argv[5],"%d",&k) != 1 || k < 0)
  {
    printf("%s is no valid threshold parameter!\n", argv[5]);
  }

  if (sscanf(argv[6],"%d",&pruning) != 1 || (pruning != 0 && pruning != 1))
  {
    printf("%s is no valid pruning option parameter (0 or 1)!\n", argv[6]);
  }
  env(w,alphabet,q,k,sm, pruning);

  delete_scorematrix(sm);

  return 0;
}
