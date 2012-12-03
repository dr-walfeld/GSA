/*
   Aufgabe 7.2

   Liest die ersten Eintraege zweier (Multi)Fasta-Dateien ein
   und berechnet das optimale Alignment mit dem Smith-Waterman-Algorithmus; 
   ein optimales Alignment wird ausgegeben;

   als Parameter werden zwei FASTA-Dateien und die Scorematrix und die
   Gapkosten angegeben
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "fasta.h"
#include "optalign.h"
#include "scorematrix.h"

int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 5)
  {
    printf("usage: %s indelscore scorematrix fasta1 fasta2\n", argv[0]);
    return 0;
  }

  // read both FASTA files
  int indel = INF;
  sscanf (argv[1], "%d", &indel);
  multifasta* mf1 = read_fasta_file (argv[3]);
  multifasta* mf2 = read_fasta_file (argv[4]);
  scorematrix* sm = read_scorematrix (argv[2], indel);

  if (indel == INF)
  {
    printf ("%s is no valid integer!\n", argv[1]);
    return 1;
  }

  if (mf1 == NULL || mf2 == NULL || sm == NULL)
    return 1;

  char* s1 = mf1->entries[0]->sequence;
  int len1 = mf1->entries[0]->length;
  char* s2 = mf2->entries[0]->sequence;
  int len2 = mf2->entries[0]->length;
  char* h1 = mf1->entries[0]->header;
  char* h2 = mf2->entries[0]->header;

  // print header
  printf("Sequenz 1: %s\n", h1);
  printf("Sequenz 2: %s\n", h2);
  // reserve memory for dynamic programing table
  alignentry*** table = initializeDP (len1+1, len2+1);
  int imax,jmax;
  int score = align (table, s1, len1, s2, len2, sm, &imax, &jmax);
  
  printf("Alignment-Score: %d\n", score);

  alignment* align = alignment_new(s1,len1,s2,len2);
  traceback(table,align,imax,jmax);
  printf("Alignment-Score (check): %d\n", alignment_evalscore(align,sm));
  if (alignment_show(align))
  {
    puts("FEHLER: inkonsistentes Alignment!");
  }
  alignment_delete(align);
  multifasta_delete(mf1);
  multifasta_delete(mf2);
  deleteDP (table, len1+1, len2+1);
  delete_scorematrix (sm);
  return 0;
}
