#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "pssm.h"
#include "fasta.h"

/* calc intermediate threshold score */
int* calc_intermediate(Pssm* pssm, int th)
{
  int d;
  int lenpssm = pssm->n;
  int* thd = malloc(lenpssm*sizeof(int));

  /* last intermediate threshold is threshold score itself */
  d = lenpssm-1;
  thd[d] = th;
  printf("%d, ", thd[d]);

  /* calculate next threshold-score with
     maximum at position d+1 */
  for (d = lenpssm-2; d >= 0; d--)
  {
    thd[d] = thd[d+1] - get_pssmmaxscore(pssm,d+1);
    printf("%d, ", thd[d]);
  }
  printf("\n");

  return thd;
}

/* print positions in seq matching PSSM (using lookahead) */
void show_positions_look(Pssm* pssm, char* seq, int lenseq, int th, int* counter)
{
  int j,d, score;
  int lenpssm = pssm->n;

  int* thd;

  *counter = 0;

  /* calculate intermediate threshold scores */
  thd = calc_intermediate(pssm, th);

  /* calculate score for each position with PSSM;
     stop if intermediate threshold is not reached */
  for (j = 0; j < lenseq-lenpssm+1; j++)
  {
    score = 0;
    for (d = 0; d < lenpssm; d++)
    {
      score += get_pssmscore(pssm, d, seq[j+d]);
      (*counter)++;
      if (score < thd[d])
      {
        break;
      }
    }
    if (score >= th)
    {
      printf("match at position %d with score %d\n", j, score);
    }
  }

  free(thd);
}

/* print positions in seq matching PSSM */
void show_positions_naive(Pssm* pssm, char* seq, int lenseq, int th, int* counter)
{
  int j,d, score;
  int lenpssm = pssm->n;

  *counter = 0;

  for (j = 0; j < lenseq-lenpssm+1; j++)
  {
    /* calculate score for each position with PSSM */
    score = 0;
    for (d = 0; d < lenpssm; d++)
    {
      score += get_pssmscore(pssm, d, seq[j+d]);
      (*counter)++;
    }
    if (score >= th)
    {
      printf("match at position %d with score %d\n", j, score);
    }
  }
}

int main(int argc, char* argv[])
{
  char *seq;
  int th, lenseq, counter;

  if (argc < 4)
  {
    fprintf(stderr,"usage: %s pssmfile seq th\n", argv[0]);
    return 0;
  }

  /* open pssmfile */
  Pssm* pssm = read_pssm(argv[1]);

  if (pssm == NULL)
    return 1;

  /* open sequence file */
  multifasta* mf = read_fasta_file(argv[2]);
  if (mf == NULL)
  {
    delete_pssm(pssm);
    return 1;
  }

  printf("%s\n", mf->entries[0]->header);
  seq = mf->entries[0]->sequence;
  lenseq = mf->entries[0]->length;

  if (sscanf(argv[3],"%d",&th) != 1)
  {
    fprintf(stderr, "ERROR: %s is no valid value for threshold!\n", argv[3]);
    return 1;
  }

  /* calculate positions matching pssm */
  show_positions_naive(pssm, seq, lenseq, th, &counter);
  printf("%d scorings\n", counter);
  show_positions_look(pssm, seq, lenseq, th, &counter);
  printf("%d scorings\n", counter);

  delete_pssm(pssm);
  multifasta_delete(mf);

  return 0;
}
