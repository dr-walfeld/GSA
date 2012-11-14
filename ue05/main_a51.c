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

  int i,j;
  int size = mf->length;
  int** alignmatrix = (int**) malloc(size*sizeof(int*));
  for (i = 0; i < size; i++)
  {
    alignmatrix[i] = (int*) calloc(size,sizeof(int));
  }

  for (i = 0; i < size; i++)
  {
    s1 = mf->entries[i]->sequence;
    len1 = mf->entries[i]->length;
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

  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      printf("%5d ", alignmatrix[i][j]);
    }
    printf("\n");
  }

  for (i = 0; i < size; i++)
  {
    free (alignmatrix[i]);
  }
  free (alignmatrix);

  multifasta_delete(mf);

  return 0;
}
