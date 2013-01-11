#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "encode.h"
#include "linkedlist.h"

/* create list of positions of words in integer-encoded sequence */
linkedlist** create_hash(int* encseq, int lenseq, int lenhash)
{
  linkedlist** khash = malloc(lenhash*sizeof(linkedlist*));
  int i;
  /* set all entries to NULL */
  for (i = 0; i < lenhash; i++)
  {
    khash[i] = NULL;
  }
  for (i = 1; i <= lenseq; i++)
  {
    if (khash[encseq[i-1]] == NULL)
      khash[encseq[i-1]] = linked_list_new();
    linked_list_add(khash[encseq[i-1]],i);
  }

  return khash;
}

/* delete hash */
void delete_hash(linkedlist** h, int len)
{
  if (h == NULL)
    return;
  int i;
  for (i = 0; i < len; i++)
  {
    if (h[i] != NULL)
      linked_list_delete(h[i]);
  }
  free(h);
  h = NULL;
}

/* calculate fasta-score of query-sequence w (of length n, represented by hwc)
   on database sequence u (of length m) with alphabet A (of length r) and
   wordlength q; use precalculated powers rpot [0..r^q-1] */
int calcscorefasta(char* u, int m, linkedlist** hwc, int n, int* A, int r, int q, int* rpot)
{
  /* allocate memory for count array [-m..n] */
  int* count = calloc(m+n+1,sizeof(int));
  int j,i;
  int max = 0;
  int c = 0;
  int fail = 0;

  /* iterate over all q-words of databe u */
  for (j = 1; j <= m-q+1; j++)
  {
    /* calculate first q-word in O(q) */
    if (j == 1)
    {
      for (i = 1; i <= q; i++)
      {
        c += translate(u[i-1],A,&fail)*rpot[q-i];
        if (fail)
          return -1;
      }
    }
    /* calculate following q-words in constant time */
    else
    {
      c = (c - translate(u[j-1-1],A,&fail)*rpot[q-1])*r +
        translate(u[j+q-1-1],A,&fail);
      if (fail)
        return -1;
    }

    /* if current q-word in u occured in w (a positions list exists)
       => check all positions in that list an increase on respective diagonal */
    if (hwc[c] != NULL)
    {
      llentry* curr;
      for (curr = hwc[c]->root; curr != NULL; curr = curr->next)
      {
        i = curr->key;
        /* check if value current value (after incrementation) is larger than
           max */
        if (++count[i-j+m] > max)
          max = count[i-j+m];
      }
    }

  }

  /* optional: print score of each diagonal */
  for (i = -m; i <= n; i++)
  {
    printf("%d:%d,", i, count[i+m]);
  }
  printf("\n");

  free(count);

  return max;
}

/* calculate fastascore for database-sequence u, query sequence w, alphabet and
   wordlength q */
int fastascore(char* u, char* w, char* alphabet, int q)
{
  int m = strlen(u);
  int n = strlen(w);

  /* create new alphabet table */
  int* A = new_alphabet(alphabet);
  if (A == NULL)
    return -1;

  int r = strlen(alphabet);

  /* encode query sequence as list of integer codes */
  int* encw = translate_sequence(w,n,q,r,A);

  /* precalculate list of 0..r^q-1 */
  int* rpot = get_pot_list(r,q);

  /* create new positions-table (hash) for query sequence w */
  linkedlist** hwc = create_hash(encw,n-q+1,rpot[q]);

  /* calculate score with database u and positions-table hwc */
  int score = calcscorefasta(u,m,hwc,n,A,r,q,rpot);

  delete_hash(hwc,rpot[q]);
  free(encw);
  free(A);
  free(rpot);

  return score;
}

int main(int argc, char* argv[])
{
  if (argc < 5)
  {
    printf("usage: %s u w A q\n", argv[0]);
    return 0;
  }

  char* u = argv[1];
  char* w = argv[2];
  char* alphabet = argv[3];
  int q;
  if (sscanf(argv[4], "%d", &q) != 1 || q <= 0)
  {
    printf("ERROR: %s is no valid q-length!\n", argv[4]);
    return 1;
  }

  int score = fastascore(u,w,alphabet,q);

  printf("score: %d\n", score);
  return 0;
}
