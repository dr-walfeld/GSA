#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"
#include "scorematrix.h"
#include "trie.h"
#include "fasta.h"
#include "encode.h"

/* function NEW in 12.2 */
int* calclookahead(char* seq, char* alphabet, int r, int q, int k, scorematrix* sm)
{
  int* look = malloc(sizeof(int)*(q));
  int d;

  /* calculate threshold level for each position in q-word */

  /* last position: threshold itself */
  look[q-1] = k;
  /* calculate threshold level d with maximum score of
     character at position d+1 */
  for (d = q-2; d >= 0; d--)
  {
    look[d] = look[d+1] - get_maxscore(sm,seq[d+1]);
  }

  return look;
}

/* construct alignment as linkedlist, save alignment as integer
   code and show alignment
*/
void construct_environment(linkedlist* stack, char* alphabet, int q, int pos, int* A, int* rpot, linkedlist** environment)
{
  TrieElement* elem;
  char envstring[q+1];
  int translated;

  envstring[q] = '\0';

  while(!linked_list_is_empty(stack))
  {
    elem = (TrieElement*) linked_list_pop(stack);
    while(elem->parent != NULL)
    {
      envstring[elem->dephth-1] = alphabet[elem->edge];
      elem = elem->parent;
    }
    printf("(%s,%d)\n", envstring, pos);
    translated = translate_qword(envstring, q, A, rpot);
    
    if (translated == -1)
    {
      return;
    }
    if (environment[translated] == NULL)
    {
      environment[translated] = linked_list_new();
    }
    linked_list_add(environment[translated], pos, (void*)NULL);
  }

}


/* calculate environment of seq with alphabet (length r) and w-word-length q,
   threshold level k and scorematrix sm */
void calcenv(char* seq, int pos, char* alphabet, int r, int q, int k, scorematrix* sm, int* pushes, int pruning, int* A, int* rpot, linkedlist** environment)
{
  linkedlist *tempstack, *environmentstack;
  TrieElement *curr,*parent;
  int i, score, *look;
  Trie* t;

  tempstack = linked_list_new();
  environmentstack = linked_list_new();
  if (pruning)
    look = calclookahead(seq, alphabet, r, q, k, sm); /* NEW in 12.2 */
  else
    look = NULL;

  /* generate new Trie */
  t = trie_new();
  t->root = add_new_trieelement(NULL,NULL,0,0);
  linked_list_push(tempstack,0,(void*)t->root);
  (*pushes)++;

  while(!linked_list_is_empty(tempstack))
  {
    parent = (TrieElement*) linked_list_pop(tempstack);
    curr = NULL;
    if (parent->dephth < q)
    {
      for (i = 0; i < r; i++)
      {
        /* calculate score */
        score = get_score(sm, alphabet[i], seq[parent->dephth]);
        /* add new node for each possible character in alphabet */
        /* if-condition NEW in 12.2 */
        if (pruning == 0 || parent->key + score >= look[parent->dephth])
        {
          curr = add_new_trieelement(parent,curr,parent->key + score, i);
          linked_list_push(tempstack,0,(void*)curr);
          (*pushes)++;
        }
      }
    }
    /* check if leaves are above threshold */
    else if (parent->key >= k)
    {
      linked_list_push(environmentstack, 0, (void*) parent);
    }

  }

  construct_environment(environmentstack,alphabet,q,pos,A,rpot,environment);

  /* free memory */
  trie_delete(t);
  linked_list_delete(tempstack);
  free(look);
  linked_list_delete(environmentstack);

}

/* 
   calculate environment for alphabet and q-word length q, threshold level k
   and scorematrix sm; pruning specifies if complete trie should be calculated
*/
linkedlist** calculate_environment(char* seq, int lenseq, char* alphabet, int lenalph, int q, int k, scorematrix* sm, int pruning, int* A, int* rpot)
{
  int i,pushes = 0;
  linkedlist** environment;

  environment = malloc(sizeof(linkedlist*)*(rpot[q]));
  for(i = 0; i < rpot[q]; i++)
  {
    environment[i] = NULL;
  }

  for(i = 0; i < lenseq-q+1; i++)
  {
    calcenv(seq+i,i,alphabet,lenalph,q,k,sm,&pushes,
        pruning,A,rpot,environment);
  }
  printf("%d Pushes to Stack!\n", pushes);

  return environment;
}

int main(int argc, char* argv[])
{
  char *alphabet, *seq;
  int q,k,pruning,lenseq,i,lenalph,*A,*rpot;
  multifasta* mf;
  linkedlist** environment;
  llentry* te;

  if (argc < 7)
  {
    printf("usage: %s scorematrix alphabet seq.fasta q k pruning\n", argv[0]);
    return 0;
  }

  scorematrix* sm = read_scorematrix(argv[1],0);
  if (sm == NULL)
    return 1;

  alphabet = argv[2];
  lenalph = strlen(alphabet);

  mf = read_fasta_file(argv[3]);
  if (mf == NULL)
  {
    delete_scorematrix(sm);
    return 1;
  }

  seq = mf->entries[0]->sequence;
  lenseq = mf->entries[0]->length;

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

  A = new_alphabet(alphabet);
  rpot = get_pot_list(lenalph,q);

  environment = calculate_environment(seq,lenseq,alphabet,lenalph,q,k,sm, pruning,A,rpot);

  for (i = 0; i < rpot[q]; i++)
  {
    if (environment[i] != NULL)
    {
      printf("%d: ", i);
      te = environment[i]->root;
      while(te != NULL)
      {
        printf("%d, ", te->key);
        te = te->next;
      }
      linked_list_delete(environment[i]);
      printf("\n");
    }
  }

  free(environment);
  delete_scorematrix(sm);
  multifasta_delete(mf);
  free(A);
  free(rpot);

  return 0;
}
