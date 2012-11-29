/*
   Berechnung des optimalen Alignments zweier Sequenzen mit
   benutzerdefinierter Kostenfunktion;
   Berechnung der beiden DP-Tabelle und Traceback;
   nutzt Alignment-Repraesentation (alignment.h) zur Speicherung
   eines optimalen Alignments;

   Compilieren mit Direktive DUNIT_TEST fuer Test;
   als Parameter zwei Sequenzen und Kostenfunktion
   (0: Unit, 1: Hamming);
   gibt DP-Matrix, Kosten des optimalen Alignments 
   und ein optimales Alignment aus;
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "optalign.h"
#include "scorematrix.h"

int get_score (scorematrix* sm, char a, char b)
{
  static scorematrix* mysm;
  if (sm != NULL)
    mysm = sm;
  return score(mysm,a,b);
}

// create new entry in alignment-table
alignentry* alignentry_new (int value, int west, int north, int northwest)
{
  alignentry* temp = (alignentry*) malloc(sizeof(alignentry));
  temp->value = value;
  temp->west = west;
  temp->north = north;
  temp->northwest = northwest;
  return temp;
}

// fill DP-table
int align (alignentry*** table, char* s1, int len1, char* s2, int len2, int (*scoreFunc)(scorematrix*,char,char), int* pimax, int* pjmax)
{
  // simultanously dertermine maximum values
  int max = -1;
  int imax = -1;
  int jmax = -1;
  int i = 0;
  int j = 0;
  int score = 0;
  int a,b,c,north,west,northwest;

  int indel = scoreFunc(NULL, '-', '-');

  // fill first columns of all rows
  for (i = 0; i <= len1; i++)
  {
    score = 0;
    table[i][0] = alignentry_new(score,0,1,0);
  }
  
  // fill first row
  for (j = 0; j <= len2; j++)
  {
    score = 0;
    table[0][j] = alignentry_new(score,1,0,0);
  }

  // iterate over matrix and calculate current element
  for (i = 1; i <= len1; i++)
  {
    for (j = 1; j <= len2; j++)
    {
      a = table[i][j-1]->value + indel;
      b = table[(i-1)][j]->value + indel;
      c = table[(i-1)][j-1]->value + scoreFunc (NULL, (s1[i-1]), (s2[j-1]));
      // maximize over a,b,c AND 0 (Smith-Waterman)
      score = MAX(MAX(MAX(0,c),b),a);
      // determine maxvalue
      if (score >= max)
      {
        max = score;
        imax = i;
        jmax = j;
      }
      north = 0;
      west = 0;
      northwest = 0;
      if (score == a)
        west = 1;
      if (score == b)
        north = 1;
      if (score == c)
        northwest = 1;
      // if 0 from nowhere => no direction neccessary
      table[i][j] = alignentry_new(score, west, north, northwest);
    }
  }

  // write imax, jmax to pointers
  *pimax = imax;
  *pjmax = jmax;

  // return max score in table
  return max;
}

// reserve memory for DP-table of size mxn and initialize with 0
// return pointer to DP-matrix
alignentry*** initializeDP (int m, int n)
{
  alignentry*** table = (alignentry ***) calloc (m, sizeof(alignentry**));
  int i;

  for (i = 0; i < m; i++)
  {
    table[i] = (alignentry **) calloc (n, sizeof(alignentry*));
  }

  return table;
}

// free memory of table
void deleteDP (alignentry*** table, int m, int n)
{
  int i,j;
  for (i = 0; i < m; i++)
  {
    for (j = 0; j < n; j++)
    {
      free (table[i][j]);
    }
    free(table[i]);
  }
  free(table);
}

int traceback (alignentry*** table, alignment* a, int i, int j)
{
  // just repeat until table[i][j] gets 0
  while (table[i][j]->value != 0)
  {
    //printf("%d,%d\n",i,j);
    // order is given
    if (table[i][j]->northwest)
    {
      alignment_add_replacement (a);
      i--;
      j--;
    }
    else if (table[i][j]->north)
    {
      alignment_add_deletion (a);
      i--;
    }
    else if (table[i][j]->west)
    {
      alignment_add_insertion (a);
      j--;
    }
  }
  alignment_reverse (a);
  return 0;
}

void show_DPtable(char* s1,int len1,char* s2,int len2, alignentry*** table)
{
  int i,j;
  int c;
  printf("------------------------------------------------------\n");
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
        printf("%3d ", c);
      }
    }
    printf("\n");
  }
  printf("------------------------------------------------------\n");
}
  
#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 5)
  {
    printf("usage: %s seq1 seq2 scorematrix indelcosts\n!", argv[0]);
    return 1;
  }
  
  // read sequences and m (cost function) from command line
  char* s1 = argv[1];
  int len1 = strlen(s1);
  char* s2 = argv[2];
  int len2 = strlen(s2);
  char* filename = argv[3];
  int indel;
  sscanf(argv[4], "%d", &indel);

  printf("Sequenz 1: %s\n", s1);
  printf("Sequenz 2: %s\n", s2);

  scorematrix* sm = read_scorematrix(filename,indel);
  if (sm == NULL)
    return 1;
  get_score(sm,'-','-');

  // reserve memory for dynamic programing table
  alignentry*** table = initializeDP (len1+1, len2+1);

  // calculate costs for optimal alignment of s1 and s2
  int imax,jmax;
  int score = align (table, s1, len1, s2, len2, get_score, &imax, &jmax);
  // print dynamic programing table
  show_DPtable(s1,len1,s2,len2,table);
  printf("Alignment: %d\n", score);

  alignment* align = alignment_new(s1,len1,s2,len2);
  traceback(table,align,imax,jmax);
  if (alignment_show(align))
  {
    puts("FEHLER: inkonsistentes Alignment!");
  }
  alignment_delete(align);
  deleteDP (table, len1+1, len2+1);

  return 0;
}
#endif
