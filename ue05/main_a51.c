#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fasta.h"
#include "alignment.h"
#include "optalign.h"

int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 3)
  {
    printf("FEHLER: Es muss eine FASTA-Datei und die Kostenfunktion angegeben werden!\n");
    return 1;
  }

  // read fasta file
  multifasta* mf = read_fasta_file (argv[1]);
  int m = atoi(argv[2]);

  if (mf == NULL)
    return 1;

  int costs = -1;
  alignentry*** table = NULL;

  char* s1 = NULL;
  int len1 = -1;
  char* s2 = NULL;
  int len2 = -1;
  //char* h1 = NULL;
  //char* h2 = NULL;

  // determine function-pointer for cost function
  int (*costFunc)(char,char);
  if (m == 0)
    costFunc = unity;
  else if (m == 1)
    costFunc = hamming;
  else
    costFunc = otherCosts;

  // initialze alignment-matrix
  int i,j;
  int size = mf->length;
  int** alignmatrix = (int**) malloc(size*sizeof(int*));
  for (i = 0; i < size; i++)
  {
    alignmatrix[i] = (int*) calloc(size,sizeof(int));
  }

  // calculate alignments => fill alignment-matrix
  for (i = 0; i < size; i++)
  {
    s1 = mf->entries[i]->sequence;
    len1 = mf->entries[i]->length;
    printf("entry %d: %s\n", i, mf->entries[i]->header);
    for (j = i+1; j < size; j++)
    {
      s2 = mf->entries[j]->sequence;
      len2 = mf->entries[j]->length;

      // reserve memory for dynamic programing table
      table = initializeDP (len1+1, len2+1);
      // calculate costs for optimal alignment of s1 and s2
      costs = align (table, s1, len1, s2, len2, costFunc);
      alignmatrix[i][j] = costs;
      alignmatrix[j][i] = costs;
      deleteDP (table, len1+1, len2+1);
    }
  }

  // print alignment-matrix
  for (i = -1; i < size; i++)
  {
    for (j = -1; j < size; j++)
    {
      if (i == -1 && j == -1)
      {
        printf("%5c ", ' ');
      }
      else if (i == -1)
      {
        printf("%4dE ", j);
      }
      else if (j == -1)
      {
        printf("%4dE ", i);
      }
      else
      {
        printf("%5d ", alignmatrix[i][j]);
      }
    }
    printf("\n");
  }

  // free alignmentmatrix
  for (i = 0; i < size; i++)
  {
    free (alignmatrix[i]);
  }
  free (alignmatrix);

  // free multifasta
  multifasta_delete(mf);

  return 0;
}
