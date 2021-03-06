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
int align (alignentry*** table, char* s1, int len1, char* s2, int len2, int (*costFunc)(char,char))
{
  int i = 0;
  int j = 0;
  int cost = 0;
  int a,b,c,north,west,northwest;

  // determine gap-costs
  int gap = costFunc ('-', '-');

  // fill first columns of all rows
  for (i = 0; i <= len1; i++)
  {
    cost = i*gap;
    // check if costs are bigger than "infinity" => costs = INF
    if (cost > INF)
      cost = INF;
    table[i][0] = alignentry_new(cost,0,1,0);
  }
  
  // fill first row
  for (j = 0; j <= len2; j++)
  {
    cost = j*gap;
    if (cost > INF)
      cost = INF;
    table[0][j] = alignentry_new(cost,1,0,0);
  }

  // iterate over matrix and calculate current element
  for (i = 1; i <= len1; i++)
  {
    for (j = 1; j <= len2; j++)
    {
      a = table[i][j-1]->value + gap;
      b = table[(i-1)][j]->value + gap;
      c = table[(i-1)][j-1]->value + costFunc ((s1[i-1]), (s2[j-1]));
      cost = MIN(a,b,c);
      north = 0;
      west = 0;
      northwest = 0;
      if (cost == a)
        west = 1;
      if (cost == b)
        north = 1;
      if (cost == c)
        northwest = 1;
      if (cost > INF)
        cost = INF;
      table[i][j] = alignentry_new(cost, west, north, northwest);
    }
  }

  // return costs for last element in table
  return cost;
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
  if (table == NULL)
    return;

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
  while (i > 0 || j > 0)
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

void show_dp(alignentry*** table, char* s1, char* s2)
{
  int len1 = strlen(s1);
  int len2 = strlen(s2);
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
 
}

alignment* optimal_alignment(char* s1, char* s2, int (*costFunc) (char, char), int* cost)
{
  int len1 = strlen(s1);
  int len2 = strlen(s2);

  // reserve memory for dynamic programing table
  alignentry*** table = initializeDP (len1+1, len2+1);
  // calculate costs for optimal alignment of s1 and s2
  *cost = align (table, s1, len1, s2, len2, costFunc);
  alignment* a = alignment_new(s1,len1,s2,len2);
  traceback(table,a,len1,len2);

  // if test-mode => show table
  #ifdef UNIT_TEST
  show_dp(table, s1, s2);
  #endif

  deleteDP (table, len1+1, len2+1);

  return a;
}

#ifdef UNIT_TEST

int unity (char a, char b)
{
  if (a == '-' || b == '-')
    return 1;
  return (a != b);
}

int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 3)
  {
    printf("usage: %s seq1 seq2\n!", argv[0]);
    return 0;
  }
  
  int costs;
  alignment* a = optimal_alignment(argv[1], argv[2], unity, &costs);

  printf("Alignment: %d\n", costs);

  if (alignment_show(a))
  {
    puts("FEHLER: inkonsistentes Alignment!");
  }
  alignment_delete(a);

  return 0;
}
#endif
