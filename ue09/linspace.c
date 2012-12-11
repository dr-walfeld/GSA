#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN(x,y) ((x < y) ? x : y)

/* we KNOW the upper left corner and the lower right corner
   are part of an optimal alignment (in the beginning trivial,
   later is i0,j0 exactly chosen to fullfill this condition);
   => therefore we can calculate the optimal alignment between
      those points and again get an optimal alignment
   => we can always start at 0 and initialize the row
      with gap costs
*/
void firstEDRcolumn(int* E, int* R, char* u, int ui, int p)
{
  E[ui-1] = 0;
  R[ui-1] = 0;
  //printf("%d,", E[ui-1]);
  int i;
  for (i = ui; i <= ui+p+1; i++)
  {
    E[i] = E[i-1] + 1;
    //printf("%d,", E[i]);
    R[i] = 0;
  }
  //printf("\n");
}

void nextEDRcolumn(int* E, int* R, int j0, char* u, int ui, int p, char vci, int j)
{

  int a,b,c,temp,tempR,cost;
  int newR = 0;
  temp = E[ui-1];
  tempR = R[ui-1];
  // gap costs for j = 0
  E[ui-1] = E[ui-2] + 1;
  //printf("%d,", E[ui]);

  int i;
  for (i = 0; i <= p; i++)
  {
    a = temp + (u[i+ui-1] != vci);
    b = E[i+ui] + 1;
    c = E[i+ui-1] + 1;

    cost = MIN(a,MIN(b,c));
    //printf("(%d;%d;%d => %d)", a,b,c,cost);
    if (j == j0)
    {
      newR = i+1;
    }
    else if (j > j0)
    {
      if (cost == a)
        newR = tempR;
      else if (cost == b)
        newR = R[i+ui];
      else
        newR = R[i+ui-1];
    }
    // j < 0 undefined

    tempR = R[i+ui];
    R[i+ui] = newR;
    //printf("%d\n", R[i+ui]);

    temp = E[i+ui];
    E[i+ui] = cost;
    //printf("%d,", E[i+ui]);
  }
  //printf("\n");
}

void evaluateallcolumns(int* E, int* R, int j0, char* u, int ui, int p, char* v, int vj, int q)
{
  firstEDRcolumn(E,R,u,ui,p);
  int j;
  for (j = 1; j <= q+1; j++)
  {
    nextEDRcolumn(E,R,j0,u,ui,p,v[vj+j-1],j);
  }
}

void evaluatecrosspoints(int* E, int* R, int* C, char* u, int i, int p, char* v, int j, int q)
{
  int j0,i0;
  if (q >= 2)
  {
    j0 = q/2;
    evaluateallcolumns(E,R,j0,u,i,p-1,v,j,q-1);
    i0 = R[p];
    C[j+j0-1] = i+i0-1;
    evaluatecrosspoints(E,R,C,u,i,i0,v,j,j0);
    evaluatecrosspoints(E,R,C,u,i+i0,p-i0,v,j+j0,q-j0);
  }
  else
  {
    // else fall
  }
}

alignment* construct_alignment(int* C,char*u,int m,char* v, int n)
{
  int i = m;
  int j;

  alignment* a = alignment_new(u,m,v,n);

  while (i > 0)
  {
    // if row index changes
    if (C[i] != C[i-1])
    {
      // if bases are equal => add replacement
      if (v[i-1] == u[C[i]-1])
      {
        alignment_add_replacement(a);
      }
      // if bases not equal => add all deletions
      // (difference between row indices!!! (may be 1 or more))
      else
      {
        alignment_add_deletions(a,C[i]-C[i-1]);
      }
    }
    // if row index is not changing => add insertion
    else
    {
      alignment_add_insertion(a);
    }
    i--;
  }


  alignment_reverse (a);

  return a;
}

int main(int argc, char* argv[])
{

  char* seq1 = "agtgcacgt";
  char* seq2 = "agcac";

  int len1 = strlen(seq1);
  int len2 = strlen(seq2);

  char* u;
  char* v;
  int m, n;

  if (MIN(len1,len2) == len1)
  {
    m = len1;
    u = seq1;
    n = len2;
    v = seq2;
  }
  else
  {
    m = len2;
    u = seq2;
    n = len1;
    v = seq1;
  }
    
  int* E = (int*) malloc((m+1)*sizeof(int));
  int* R = (int*) malloc((m+1)*sizeof(int));

  int* C = (int*) malloc((n+1)*sizeof(int));
  C[0] = 0;
  C[n] = m;
  //evaluateallcolumns(E,R,n/2,u,0,m-1,v,0,n-1);
  evaluatecrosspoints(E,R,C,u,1,m,v,1,n);

  alignment* a = construct_alignment(C,u,m,v,n);
  alignment_show (a);

  int i;
  for (i = 0; i <= n; i++)
  {
    printf("%d,%d,", i,C[i]);
  }
  printf("\n");

  free (E);
  free (R);
  alignment_delete(a);

  return 0;
}