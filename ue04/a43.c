#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARGMIN(a,vA,b,vB,c,vC) (vA <= vB ? (vA <= vC ? a : c) : (vB <= vC ? b : c))
#define INF 1000000

int align (int*, char*, int, char*, int, int (*) (int,int,char,char));
int calcCost (int*, int, char, int, char, int, int (*)(int,int,char,char));
int unity (int, int, char, char);
int hamming (int, int, char, char);
int otherCosts (int, int, char, char);

int unity (int i, int j, char c1, char c2)
{
  if (i == 0 || j == 0)
    return 1;
  if (c1 != c2)
    return 1;
  return 0;
}

int hamming (int i, int j, char c1, char c2)
{
  if (i == 0 || j == 0)
    return INF;
  if (c1 != c2)
    return 2;
  return 0;
}

int otherCosts (int i, int j, char c1, char c2)
{
  if (i == 0 || j == 0)
    return 1;
  if (c1 != c2)
    return 3;
  return 0;
}

int calcCost (int* table, int i, char c1, int j, char c2, int len2, int (*costFunc)(int,int,char,char))
{
  int a,vA,b,vB,c,vC,cost;
  int gap = costFunc (0,0,c1,c2);
  int base = costFunc (i,j,c1,c2);
  if (i == 0 && j == 0)
  {
    cost = 0;
  }
  else if (i == 0)
  {
    cost = table[j-1] + gap;
  }
  else if (j == 0)
  {
    cost = table[(i-1)*(len2+1)] + gap;
  }
  else
  {
    // Implementierung nicht direkt ueber Wert, sondern ueber
    // index => nuetzlich fuer backtracking
    a = i*(len2+1)+j-1;
    b = (i-1)*(len2+1)+j;
    c = (i-1)*(len2+1)+j-1;
    vA = table[a] + gap;
    vB = table[b] + gap;
    vC = table[c] + base;
    int minindex = ARGMIN(a,vA,b,vB,c,vC);
    if (minindex == a)
    {
      cost = vA;
    }
    else if (minindex == b)
    {
      cost = vB;
    }
    else
    {
      cost = vC;
    }
  }
  // if costs are bigger than infinity (INF) => cost = INF
  if (cost > INF)
    cost = INF;
  return cost;
}

// calculate alignment
int align (int* table, char* s1, int len1, char* s2, int len2, int (*costFunc)(int,int,char,char))
{
  int i = 0;
  int j = 0;
  int cost = 0;

  // iterate over matrix and calculate current element
  for (i = 0; i <= len1; i++)
  {
    for (j = 0; j <= len2; j++)
    {
      cost = calcCost (table, i, (i > 0 ? s1[i-1] : 'X'), j, (j > 0 ? s2[j-1] : 'X'), len2, costFunc);
      table[i*(len2+1)+j] = cost;
    }
  }

  // return costs for last element in table
  return table[(i-1)*(len2+1)+j-1];
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
  int (*costFunc)(int,int,char,char);
  if (m == 0)
    costFunc = unity;
  else if (m == 1)
    costFunc = hamming;
  else
    costFunc = otherCosts;

  printf("Sequenz 1: %s\n", s1);
  printf("Sequenz 2: %s\n", s2);
  // reserve memory for dynamic programing table
  int* table = (int *) malloc((len1+1)*(len2+1)*sizeof(int));
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
      c = table[i*(len2+1)+j];
      if (c == INF)
        c = -1;
      printf("%2d ", c);
    }
    printf("\n");
  }
  printf("------------------------------------------------------\n");
  
  // backtracking for printing the optimal alignment
  printf("Alignment: %d\n", costs);

  return 0;
}
