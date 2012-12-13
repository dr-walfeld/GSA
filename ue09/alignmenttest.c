#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "optalign.h"
#include "linspace.h"
#include "fasta.h"

int main(int argc, char* argv[])
{
  // check argument count
  if (argc < 3)
  {
    printf("usage: %s fasta1 fasta2 ...\n", argv[0]);
    return 0;
  }

  // read multifasta-file
  multifasta** fastafiles = (multifasta**) malloc((argc-1)*sizeof(multifasta*));
  int i;
  for (i = 1; i < argc; i++)
  {
    fastafiles[i-1] = read_fasta_file(argv[i]);
    if (fastafiles[i-1] == NULL)
      return 1;
  }

  char* s1;
  int len1;
  char* h1;
  char* s2;
  int len2;
  char* h2;
  alignentry*** table;
  int costs,costs_lin;
  alignment* align1;
  alignment* align2;
  char* u;
  char* v;


  // calculate alignment between all sequences
  int j;
  for (i = 0; i < argc-1; i++)
  {
    for (j = 0; j < argc-1; j++)
    {
      s1 = fastafiles[i]->entries[0]->sequence;
      len1 = fastafiles[i]->entries[0]->length;
      h1 = fastafiles[i]->entries[0]->header;
      s2 = fastafiles[j]->entries[0]->sequence;
      len2 = fastafiles[j]->entries[0]->length;
      h2 = fastafiles[j]->entries[0]->header;

      // reserve memory for dynamic programing table
      table = initializeDP (len1+1, len2+1);
      // calculate costs for optimal alignment of s1 and s2
      costs = align (table, s1, len1, s2, len2, unity);
      align1 = alignment_new(s1,len1,s2,len2);
      traceback(table,align1,len1,len2);
      printf("alignment: %d (eval: %d)\n",costs, alignment_evalcost(align1,
            unity));
      alignment_show(align1);


      align2 = linear_space_alignment(s1,s2,unity,&costs_lin);
      printf("alignment (linear): %d (eval: %d)\n", costs_lin,
        alignment_evalcost(align2,unity));
      alignment_show(align2);

      // compare costs
      if (costs != costs_lin)
      {
        printf("costs not matching!\n");
      }
      // compare alignments
      if (!alignment_compare(align1,align2))
      {
        printf("alignments not matching!\n");
      }

      // free memory of both alignments and DP-table
      alignment_delete(align2);
      alignment_delete(align1);
      deleteDP (table, len1+1, len2+1);
    }
  }

  // free memory of multifasta
  for (i = 1; i < argc; i++)
  {
    multifasta_delete(fastafiles[i-1]);
  }

  return 0;
}
