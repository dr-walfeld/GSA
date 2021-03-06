#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "fasta.h"
#include "msa.h"

/* validate MSA;
   returns 0 is valid
*/
static int msa_validate (Msa* msa)
{
  int i,j,len,nogap;

  /* multiple alignments must contain at least 2 sequences */
  if (msa->length < 2)
  {
    return 1;
  }

  len = msa->entries[0]->length;
  /* check if all s' have the same length */
  for (i = 1; i < msa->length; i++)
  {
    if (msa->entries[i]->length != len)
      return 1;
  }

  /* check if there is one sequence at each position
     without a gap
  */
  for (i = 0; i < len; i++)
  {
    nogap = 0;
    for (j = 0; j < msa->length; j++)
    {
      if(msa->entries[j]->sequence[i] != '-')
        nogap = 1;
    }
    if (!nogap)
      return 1;
  }
  return 0;
} 

/* initialize MSA (read from file) */
Msa* msa_initialize (char* filename)
{
  Msa* msa;

  msa = malloc(sizeof(Msa));
  assert(msa);

  msa->mf = read_fasta_file(filename);
  if (msa->mf == NULL)
  {
    free(msa);
    return NULL;
  }
  msa->entries = msa->mf->entries;

  msa->length = msa->mf->length;

  /* check validity */
  if (msa_validate(msa))
  {
    fprintf(stderr,"ERROR: invalid multiple sequence alignment!\n");
    msa_delete(msa);
    return NULL;
  }

  return msa;
}

/* free memory */
void msa_delete (Msa* msa)
{
  multifasta_delete(msa->mf);
  free(msa);
}

/* print out msa on stdout */
static void msa_show(Msa* msa)
{
  int i;
  for(i = 0; i < msa->length; i++)
  {
    printf(">%s\n",msa->entries[i]->header);
    printf("%s\n",msa->entries[i]->sequence);
  }
}

/* calculate sum of pairs score for msa with score function score;
   scorematrix may be NULL (if not needed)
*/
int msa_sum_of_pairs(Msa* msa, int(*score)(void*, char, char), void* scorematrix)
{
  int i,j,k,sc=0;


  for (i = 0; i < msa->entries[0]->length; i++)
  {
    for (j = 0; j < msa->length; j++)
    {
      for (k = j+1; k < msa->length; k++)
      {
        sc += score(scorematrix, msa->entries[j]->sequence[i],
            msa->entries[k]->sequence[i]);
      }
    }
  }

  return sc;
}

/* calculate consus score for msa;
   scorematrix may be NULL if not needed 
*/
int msa_consensus_score(Msa* msa, int (*score)(void*, char, char), void* scorematrix)
{
  int i,j,sc=0,*count,maxval;
  char cons,pos;

  count = malloc(UCHAR_MAX*sizeof(int));

  printf("\n");
  /* iterate over all positions of alignment */
  for (i = 0; i < msa->entries[0]->length; i++)
  {
    maxval = -1;
    cons = 0;

    /* set count of all character to 0 */
    for (j = 0; j < UCHAR_MAX; j++)
    {
      count[j] = 0;
    }

    /* 
       count characters at current positions and determine max (i.e. consensus)
    */
    for (j = 0; j < msa->length; j++)
    {
      pos = msa->entries[j]->sequence[i];
      /* count everything except gap */
      if (pos != '-')
      {
        count[(int)pos]++;
        if (count[(int)pos] >= maxval)
        {
          maxval = count[(int)pos];
          cons = pos;
        }
      }
    }
    printf("%c",cons);
    /* score each sequence at current position against consensus */
    for (j = 0; j < msa->length; j++)
    {
      sc += score(scorematrix,cons,msa->entries[j]->sequence[i]);
    }
  }

  printf("\n");

  free(count);

  return sc;
}

#ifdef UNIT_TEST
/* test score function */
static int unit_score(void* scorematrix, char a, char b)
{
  return (a != b);
}

int main(int argc, char* argv[])
{
  Msa* msa;
  int sop,cons;

  if (argc < 2)
  {
    fprintf(stderr,"usage: %s msa.fasta\n", argv[0]);
    return 0;
  }

  msa = msa_initialize(argv[1]);
  if (msa == NULL)
    return 0;

  msa_show(msa);

  sop = msa_sum_of_pairs(msa,unit_score,NULL);
  cons = msa_consensus_score(msa,unit_score,NULL);
  printf("Consensus: %d\nSum of pairs: %d\n", cons,sop);

  msa_delete(msa);

  return 0;
}
#endif
