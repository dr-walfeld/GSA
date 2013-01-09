#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "encode.h"
#include "linkedlist.h"

linkedlist** create_hash(int* encseq, int lenseq, int lenhash)
{
  linkedlist** khash = malloc(lenhash*sizeof(linkedlist*));
  int i;
  for (i = 0; i < lenhash; i++)
  {
    khash[i] = linked_list_new();
  }
  for (i = 1; i <= lenseq; i++)
  {
    linked_list_add(khash[encseq[i-1]],i);
  }

  return khash;
}

void delete_hash(linkedlist** h, int len)
{
  if (h == NULL)
    return;
  int i;
  for (i = 0; i < len; i++)
  {
    linked_list_delete(h[i]);
  }
  free(h);
  h = NULL;
}

int scorefasta(char* u, int m, linkedlist** hwc, int n, int* A, int r, int q)
{
  int* count = calloc(m+n+1,sizeof(int));
  int j,i;
  int max = 0;
  int c = 0;
  int fail = 0;

  int* rpot = get_pot_list(r,q);

  for (j = 1; j <= m-q+1; j++)
  {
    if (j == 1)
    {
      for (i = 1; i <= q; i++)
      {
        c += translate(u[i-1],A,&fail)*rpot[q-i];
        if (fail)
          return -1;
      }
    }
    else
    {
      c = (c - translate(u[j-1-1],A,&fail)*rpot[q-1])*r +
        translate(u[j+q-1-1],A,&fail);
      if (fail)
        return -1;
    }

    llentry* curr;
    for (curr = hwc[c]->root; curr != NULL; curr = curr->next)
    {
      i = curr->key;
      if (++count[i-j+m] > max)
        max = count[i-j+m];
    }

  }

  for (i = -m; i <= n; i++)
  {
    printf("%d:%d,", i, count[i+m]);
  }
  printf("\n");

  free(rpot);
  free(count);

  return max;
}

int main(int argc, char* argv[])
{
  if (argc < 5)
  {
    printf("usage: %s u w A q\n", argv[0]);
    return 0;
  }

  char* u = argv[1];
  int m = strlen(u);
  char* w = argv[2];
  int n = strlen(w);
  char* alphabetstring = argv[3];
  int q;
  if (sscanf(argv[4], "%d", &q) != 1 || q <= 0)
  {
    printf("ERROR: %s is no valid q-length!\n", argv[4]);
    return 1;
  }

  int* A = new_alphabet(alphabetstring);
  if (A == NULL)
    return 1;
  int r = strlen(alphabetstring);

  int* out = translate_sequence(w,n,q,r,A);

  /*  
  int i;
  for (i = 0; i < strlen(w)-q+1; i++)
  {
    printf("%d,", out[i]);
  }
  printf("\n");
  */
  int* rpot = get_pot_list(r,q);
  linkedlist** hwc = create_hash(out,n-q+1,rpot[q]);
  int score = scorefasta(u,m,hwc,n,A,r,q);
  printf("score: %d\n", score);
  delete_hash(hwc,rpot[q]);
  free(out);
  free(A);
  free(rpot);
  return 0;
}
