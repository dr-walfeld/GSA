/*

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "optalignaffine.h"
#include "fasta.h"

int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 5)
  {
    printf("usage: %s seq1.fasta seq2.fasta gapopen gapextend\n!",argv[0]);
    return 1;
  }
  
  multifasta* mf1 = read_fasta_file (argv[1]);
  multifasta* mf2 = read_fasta_file (argv[2]);
  if (mf1 == NULL || mf2 == NULL)
        return 1;

  char* s1 = mf1->entries[0]->sequence;
  int len1 = mf1->entries[0]->length;
  char* s2 = mf2->entries[0]->sequence;
  int len2 = mf2->entries[0]->length;
  //char* h1 = mf1->entries[0]->header;
  //char* h2 = mf2->entries[0]->header;

  int open, extend;

  if (sscanf(argv[3],"%d",&open) != 1 || open < 0)
  {
    printf("invalid input for gap opening costs: %s", argv[3]);
    return 1;
  }
  if (sscanf(argv[4],"%d",&extend) != 1 || extend < 0)
  {
    printf("invalid input for gap extension costs: %s", argv[4]);
    return 1;
  }

  // reserve memory for dynamic programing table
  alignentry*** R = initializeDP (len1+1, len2+1);
  alignentry*** D = initializeDP (len1+1, len2+1);
  alignentry*** I = initializeDP (len1+1, len2+1);
  // calculate costs for optimal alignment of s1 and s2
  /*int costs = */align (R, D, I, s1, len1, s2, len2, open, extend);
  
  //printf("Alignment-Kosten: %d\n", costs);

  alignment* align = alignment_new(s1,len1,s2,len2);
  traceback(R,D,I,align,len1,len2);
  if (alignment_show(align))
  {
    puts("FEHLER: inkonsistentes Alignment!");
  }
  alignment_delete(align);
  deleteDP (R, len1+1, len2+1);
  deleteDP (D, len1+1, len2+1);
  deleteDP (I, len1+1, len2+1);
  multifasta_delete(mf1);
  multifasta_delete(mf2);
     
  return 0;
}