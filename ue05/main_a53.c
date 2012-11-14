#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "fasta.h"
#include "optalign.h"

int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 4)
  {
    printf("FEHLER: Es muessen zwei FASTA-Dateien und die Kostenfunktion angegeben werden!\n");
    return 1;
  }

  int m = atoi(argv[3]);
  multifasta* mf1 = read_fasta_file (argv[1]);
  multifasta* mf2 = read_fasta_file (argv[2]);

  if (mf1 == NULL || mf2 == NULL)
    return 1;

  char* s1 = mf1->entries[0]->sequence;
  int len1 = mf1->entries[0]->length;
  char* s2 = mf2->entries[0]->sequence;
  int len2 = mf2->entries[0]->length;
  char* h1 = mf1->entries[0]->header;
  char* h2 = mf2->entries[0]->header;

  // determine function-pointer for cost function
  int (*costFunc)(char,char);
  if (m == 0)
    costFunc = unity;
  else if (m == 1)
    costFunc = hamming;
  else
    costFunc = otherCosts;

  printf("Sequenz 1: %s\n%s\n", h1,s1);
  printf("Sequenz 2: %s\n%s\n", h2,s2);
  // reserve memory for dynamic programing table
  alignentry*** table = initializeDP (len1+1, len2+1);
  // calculate costs for optimal alignment of s1 and s2
  int costs = align (table, s1, len1, s2, len2, costFunc);
  // print dynamic programing table
  printf("------------------------------------------------------\n");
  int i,j;
  int c;
  for (i = -1; i <= len1; i++)
  {
    for (j = -1; j <= len2; j++)
    {
      if (i == -1 && j != -1)
      {
        if (j == 0)
          printf("%3c ", '-');
        else
          printf("%3c ", s2[j-1]);
      }
      else if (j == -1 && i != -1)
      {
        if (i == 0)
          printf("%3c ", '-');
        else
          printf("%3c ", s1[i-1]);
      }
      else if (i == -1 && j == -1)
      {
        printf("%3c ", ' ');
      }
      else
      {
        c = table[i][j]->value;
        if (c == INF)
          printf("%3c ", 'I');
        else
          printf("%3d ", c);
      }
    }
    printf("\n");
  }
  printf("------------------------------------------------------\n");
  
  printf("Alignment: %d\n", costs);

  alignment* align = alignment_new(s1,len1,s2,len2);
  traceback(table,align,len1,len2);
  if (alignment_show(align))
  {
    puts("FEHLER: inkonsistentes Alignment!");
  }
  alignment_delete(align);
  multifasta_delete(mf1);
  multifasta_delete(mf2);
  deleteDP (table, len1+1, len2+1);

  return 0;
}
