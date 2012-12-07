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
#include "optalignaffine.h"

// create new entry in alignment-table
alignentry* alignentry_new (int value, int r, int d, int i)
{
  alignentry* temp = (alignentry*) malloc(sizeof(alignentry));
  temp->value = value;
  temp->r = r;
  temp->d = d;
  temp->i = i;
  return temp;
}

// helper function for generation of alignment entry
alignentry* get_entry(int a, int b, int c)
{
  int cost = MIN(a,b,c);

  int rv = 0;
  int dv = 0;
  int iv = 0;

  if (cost == a)
    rv = 1;
  if (cost == b)
    dv = 1;
  if (cost == c)
    iv = 1;

  if (cost > INF)
    cost = INF;

  return alignentry_new(cost, rv, dv, iv); 
}

// fill DP-table (three tables R,D,I)
int align (alignentry*** R, alignentry*** D, alignentry*** I, char* s1, int len1, char* s2, int len2, int open, int extend)
{
  int i = 0;
  int j = 0;
  int a,b,c;

  // initialize R(0,0) with 0 and D(0,0), I(0,0) with gap opening costs
  R[0][0] = alignentry_new(0,0,0,0);
  D[0][0] = alignentry_new(open,0,0,0);
  I[0][0] = alignentry_new(open,0,0,0);

  // fill first columns of all rows
  for (i = 1; i <= len1; i++)
  {
    R[i][0] = alignentry_new(INF,0,1,0);
    I[i][0] = alignentry_new(INF,0,1,0);
    D[i][0] = alignentry_new(D[i-1][0]->value+extend,0,1,0);
  }
  
  // fill first row
  for (j = 1; j <= len2; j++)
  {
    R[0][j] = alignentry_new(INF,0,0,1);
    D[0][j] = alignentry_new(INF,0,0,1);
    I[0][j] = alignentry_new(I[0][j-1]->value+extend,0,0,1);
  }

  // iterate over matrix and calculate current element
  for (i = 1; i <= len1; i++)
  {
    for (j = 1; j <= len2; j++)
    {
      // calculate R
      // unit costs: 0 if equal, 1 otherwise
      a = R[i-1][j-1]->value + ((s1[i-1]) != (s2[j-1]));
      b = D[i-1][j-1]->value + ((s1[i-1]) != (s2[j-1]));
      c = I[i-1][j-1]->value + ((s1[i-1]) != (s2[j-1]));

      R[i][j] = get_entry(a,b,c);

      // calculate D
      a = R[i-1][j]->value + open + extend;
      b = D[i-1][j]->value + extend;
      c = I[i-1][j]->value + open + extend;

      D[i][j] = get_entry(a,b,c);

      // calculate I
      a = R[i][j-1]->value + open + extend;
      b = D[i][j-1]->value + open + extend;
      c = I[i][j-1]->value + extend;

      I[i][j] = get_entry(a,b,c);
    }
  }
  // return costs for last element in table
  return MIN(R[len1][len2]->value,D[len1][len2]->value,I[len1][len2]->value);
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

// traceback in three tables R,D,I
int traceback (alignentry*** R,alignentry*** D,alignentry*** I, alignment* a, int i, int j)
{
  // determine minimum value
  int minval = MIN(R[i][j]->value, D[i][j]->value, I[i][j]->value);
  char curr_op = '-';
  alignentry* curr = NULL;
  if (minval == I[i][j]->value)
  {
    curr_op = 'I';
    curr = I[i][j];
  }
  else if (minval == D[i][j]->value)
  {
    curr_op = 'D';
    curr = D[i][j];
  }
  else if (minval == R[i][j]->value)
  {
    curr_op = 'R';
    curr = R[i][j];
  }

  while (i > 0 || j > 0)
  {
    // check current operation ending at current node
    if (curr_op == 'R')
    {
      alignment_add_replacement (a);
      i--;
      j--;
    }
    else if (curr_op == 'D')
    {
      alignment_add_deletion (a);
      i--;
    }
    else if (curr_op == 'I')
    {
      alignment_add_insertion (a);
      j--;
    }

    // order is given for next node to visit
    // set curr_op of next node
    if (curr->r)
    {
      curr = R[i][j];
      curr_op = 'R';
    }
    else if (curr->d)
    {
      curr = D[i][j];
      curr_op = 'D';
    }
    else if (curr->i)
    {
      curr = I[i][j];
      curr_op = 'I';
    }
  }
  alignment_reverse (a);
  return 0;
}

#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 5)
  {
    printf("usage: %s seq1 seq2 gapopen gapextend\n!",argv[0]);
    return 1;
  }
  
  char* s1 = argv[1];
  int len1 = strlen(s1);
  char* s2 = argv[2];
  int len2 = strlen(s2);

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

  printf("Sequenz 1: %s\n", s1);
  printf("Sequenz 2: %s\n", s2);
  // reserve memory for dynamic programing table
  alignentry*** R = initializeDP (len1+1, len2+1);
  alignentry*** D = initializeDP (len1+1, len2+1);
  alignentry*** I = initializeDP (len1+1, len2+1);
  // calculate costs for optimal alignment of s1 and s2
  int costs = align (R, D, I, s1, len1, s2, len2, open, extend);
  
  printf("Alignment: %d\n", costs);

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

  return 0;
}
#endif
