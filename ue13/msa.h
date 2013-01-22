/*
   implementation of parser for MSAs;
   uses fasta.h for parsing of multifasta files
*/
#ifndef MSA
#define MSA

#include "fasta.h"

typedef struct msaStruct {
  multifasta* entries; /* save entries of multiple sequence alignment with
                          multifasta */
  int length;
} Msa;

/* initialize msa (read from file) */
Msa* msa_initialize(char*);
/* free memory */
void msa_delete(Msa*);
/* calculate sum of pairs score
   with scorefunction score and scorematrix (may be NULL)
*/
int msa_sum_of_pairs(Msa*, int (*) (void*, char, char), void*);
/* calculate consensus score
   with scorefunction score and scorematrix (may be NULL)
*/
int msa_consensus_score(Msa*, int (*) (void*, char, char), void*);

#endif
