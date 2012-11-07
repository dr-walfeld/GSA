#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN(a,b,c) (a <= b ? (a <= c ? a : c) : (b <= c ? b : c))
#define INF 1000000

int align (int**, char*, int, char*, int, int (*) (char,char));
int unity (char, char);
int hamming (char, char);
int otherCosts (char, char);
int** initializeDP (int, int);
void deleteDP (int**, int, int);

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

// fill DP-table
int align (int** table, char* s1, int len1, char* s2, int len2, int (*costFunc)(char,char))
{
  int i = 0;
  int j = 0;
  int cost = 0;
  int a,b,c;

  // determine gap-costs
  int gap = costFunc ('-', '-');

  // fill first columns of all rows
  for (i = 0; i <= len1; i++)
  {
    cost = i*gap;
    // check if costs are bigger than "infinity" => costs = INF
    if (cost > INF)
      cost = INF;
    table[i][0] = cost;
  }
  
  // fill first row
  for (j = 0; j <= len2; j++)
  {
    cost = j*gap;
    if (cost > INF)
      cost = INF;
    table[0][j] = cost;
  }

  // iterate over matrix and calculate current element
  for (i = 1; i <= len1; i++)
  {
    for (j = 1; j <= len2; j++)
    {
      a = table[i][j-1] + gap;
      b = table[(i-1)][j] + gap;
      c = table[(i-1)][j-1] + costFunc (s1[i-1], s2[j-1]);
      cost = MIN(a,b,c);
      if (cost > INF)
        cost = INF;
      table[i][j] = cost;
    }
  }

  // return costs for last element in table
  return cost;
}

// reserve memory for DP-table of size mxn and initialize with 0
// return pointer to DP-matrix
int** initializeDP (int m, int n)
{
  int** table = (int **) calloc (m, sizeof(int*));
  int* temp;
  int i;

  for (i = 0; i < m; i++)
  {
    temp = (int *) calloc (n, sizeof(int));
    table[i] = temp;
  }

  return table;
}

// free memory of table
void deleteDP (int** table, int m, int n)
{
  int i;
  for (i = 0; i < m; i++)
  {
    free(table[i]);
  }
  free(table);
}

int main(int argc, char* argv[])
{
  // check argument count
  if (argc < 4)
  {
    printf("FEHLER: zu wenig Argumente angegeben!");
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
  int** table = initializeDP (len1+1, len2+1);
  // calculate costs for optimal alignment of s1 and s2
  int costs = align (table, s1, len1, s2, len2, costFunc);
  // print dynamic programing table
  printf("------------------------------------------------------\n");
  int i,j;
  int c;
  for (i = 0; i <= len1; i++)
  {
    for (j = 0; j <= len2; j++)
    {
      c = table[i][j];
      if (c == INF)
        c = -1;
      printf("%2d ", c);
    }
    printf("\n");
  }
  printf("------------------------------------------------------\n");
  
  printf("Alignment: %d\n", costs);

  deleteDP (table, len1+1, len2+1);

  return 0;
}
