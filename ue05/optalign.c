#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "optalign.h"

// unit cost function
int unity (char c1, char c2)
{
  if (c1 == '-' || c2 == '-')
    return 1;
  if (c1 != c2)
    return 1;
  return 0;
}

// hamming costs
int hamming (char c1, char c2)
{
  if (c1 == '-' || c2 == '-')
    return INF;
  if (c1 != c2)
    return 2;
  return 0;
}

// other cost function
int otherCosts (char c1, char c2)
{
  if (c1 == '-' || c2 == '-')
    return 1;
  if (c1 != c2)
    return 3;
  return 0;
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
      c = table[(i-1)][j-1]->value + costFunc (s1[i-1], s2[j-1]);
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

#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  
  // check argument count
  if (argc < 4)
  {
    printf("FEHLER: zu wenig Argumente angegeben\n!");
    return 1;
  }
  
  // read sequences and m (cost function) from command line
  char* s1 = argv[1];
  int len1 = strlen(s1);
  char* s2 = argv[2];
  int len2 = strlen(s2);
  int m = atoi(argv[3]);

  // determine function-pointer for cost function
  int (*costFunc)(char,char);
  if (m == 0)
    costFunc = unity;
  else if (m == 1)
    costFunc = hamming;
  else
    costFunc = otherCosts;

  printf("Sequenz 1: %s\n", s1);
  printf("Sequenz 2: %s\n", s2);
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
  deleteDP (table, len1+1, len2+1);

  return 0;
}
#endif
