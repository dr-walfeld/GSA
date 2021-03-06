#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARGMIN(a,vA,b,vB,c,vC) (vA <= vB ? (vA <= vC ? a : c) : (vB <= vC ? b : c))
#define INF 1000000

struct alEntry {
  int cost;
  int dir;
  int from;
};

int align (struct alEntry*, char*, int, char*, int, int (*) (int,int,char,char));
int calcCost (struct alEntry*, int, char, int, char, int, int*, int*, int (*)(int,int,char,char));
void backTrack (struct alEntry*, char*, int, char*, int);
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

int calcCost (struct alEntry* table, int i, char c1, int j, char c2, int len2, int* dir, int* from, int (*costFunc)(int,int,char,char))
{
  int a,vA,b,vB,c,vC,cost;
  int gap = costFunc (0,0,c1,c2);
  int base = costFunc (i,j,c1,c2);
  if (i == 0 && j == 0)
  {
    *dir = -1;
    *from = 'X';
    cost = 0;
  }
  else if (i == 0)
  {
    *from = 'l';
    *dir = j-1;
    cost = table[*dir].cost + gap;
  }
  else if (j == 0)
  {
    *from = 'o';
    *dir = (i-1)*(len2+1);
    cost = table[*dir].cost + gap;
  }
  else
  {
    a = i*(len2+1)+j-1;
    b = (i-1)*(len2+1)+j;
    c = (i-1)*(len2+1)+j-1;
    vA = table[a].cost + gap;
    vB = table[b].cost + gap;
    vC = table[c].cost + base;
    *dir = ARGMIN(a,vA,b,vB,c,vC);
    if (*dir == a)
    {
      *from = 'l';
      cost = vA;
    }
    else if (*dir == b)
    {
      *from = 'o';
      cost = vB;
    }
    else
    {
      *from = 'd';
      cost = vC;
    }
  }
  // if costs are bigger than infinity (INF) => cost = INF
  if (cost > INF)
    cost = INF;
  return cost;
}

// calculate alignment
int align (struct alEntry* table, char* s1, int len1, char* s2, int len2, int (*costFunc)(int,int,char,char))
{
  int i = 0;
  int j = 0;
  int cost = 0;
  int dir = -1;
  int from = -1;

  // iterate over matrix and calculate current element
  for (i = 0; i <= len1; i++)
  {
    for (j = 0; j <= len2; j++)
    {
      cost = calcCost (table, i, (i > 0 ? s1[i-1] : 'X'), j, (j > 0 ? s2[j-1] : 'X'), len2, &dir, &from, costFunc);
      table[i*(len2+1)+j].cost = cost;
      table[i*(len2+1)+j].dir = dir;
      table[i*(len2+1)+j].from = from;
    }
  }

  // return costs for last element in table
  return table[(i-1)*(len2+1)+j-1].cost;
}

// backtracking; takes both sequences and dynamic programing table as input
void backTrack (struct alEntry * table, char* s1, int len1, char* s2, int len2)
{
  int i = -1;
  int j = -1;
  // pointer to current element
  struct alEntry * element = &table[len1*(len2+1)+len2];
  // repeat until next element (minimum path) doesn't exist
  while (element->dir != -1)
  {
    // calculate coordinates from next element
    j = (element->dir)%(len2+1);
    i = (element->dir)/(len2+1);
    // the current element has index of the next element +1 (-1 for String-Index)
    if (element->from == 'l')
      printf("- %c\n", s2[j]);
    else if (element->from == 'o')
      printf("%c -\n", s1[i]);
    else
      printf("%c %c\n", s1[i], s2[j]);
    // set pointer to next element (minimum path) in table
    element = &table[element->dir];
  }

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
  struct alEntry* table = (struct alEntry *) malloc((len1+1)*(len2+1)*sizeof(struct alEntry));
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
      c = table[i*(len2+1)+j].cost;
      if (c == INF)
        c = -1;
      printf("%2d(%c) ", c, table[i*(len2+1)+j].from);
    }
    printf("\n");
  }
  printf("------------------------------------------------------\n");
  
  // backtracking for printing the optimal alignment
  printf("Alignment: %d\n", costs);
  backTrack (table, s1, len1, s2, len2);

  return 0;
}
