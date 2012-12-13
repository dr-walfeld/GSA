#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "linspace.h"

#define MIN(x,y) ((x < y) ? x : y)

/* 
   we KNOW the upper left corner and the lower right corner
   are part of an optimal alignment (in the beginning trivial,
   later is i0,j0 exactly chosen to fullfill this condition);
   => therefore we can calculate the optimal alignment between
      those points and again get an optimal alignment
   => we can always start at 0 and initialize the row
      with gap costs
*/
void firstEDRcolumn(int* E, int* R, char* u, int ui, int p, int (*costFunc)(char,char))
{
  E[0] = 0;
  R[0] = 0;
  int gap = costFunc('-','x');
  //printf("%d(0),", E[0]);
  int i;
  for (i = 1; i <= p; i++)
  {
    E[i] = E[i-1] + gap;
    R[i] = 0;
    //printf("%d(%d),", E[i],R[i]);
  }
  //printf("\n");
}

// calculate the next column
void nextEDRcolumn(int* E, int* R, int j0, char* u, int ui, int p, char vci, int j, int (*costFunc)(char,char))
{

  // constant number of temporary variables
  int a,b,c,temp,tempR,cost;
  int newR = 0;

  int in = costFunc('x','-');
  int del = costFunc('-','x');

  temp = E[0];
  tempR = R[0];
  // increasing gap costs
  E[0] = E[0] + in;
  //printf("%d(%d),", E[0], R[0]);

  int i;
  // calculate all rows
  for (i = 1; i <= p; i++)
  {
    // all three direction and minimum
    a = temp + costFunc(u[i+ui-2], vci);
    b = E[i] + in;
    c = E[i-1] + del;

    cost = MIN(a,MIN(b,c));

    // if j == j0 => row number
    if (j == j0)
    {
      newR = i;
    }
    else if (j > j0)
    {
      // copy row numbers for respective mininum
      // (replacement before )
      if (cost == a)
        newR = tempR;
      else if (cost == c)
        newR = R[i-1];
      else
        newR = R[i];
    }
    // j < 0 per definition undefined

    // save old value at i and copy new value
    //printf("%d(%c,%c,%d);", cost, u[ui+i-2], vci, newR);
    tempR = R[i];
    R[i] = newR;

    temp = E[i];
    E[i] = cost;
  }
  //printf("\n");
}

// evaluates all columns of current matrix (substrings u[ui..ui+p], v[vj..vj+q]
void evaluateallcolumns(int* E, int* R, int j0, char* u, int ui, int p, char* v, int vj, int q, int (*costFunc)(char,char))
{
  // initialize first column
  firstEDRcolumn(E,R,u,ui,p,costFunc);
  int j;
  // calculate all subsequent columns
  for (j = 1; j <= q; j++)
  {
    // index v [vj+j-2] because be have to correct the 1-indexing
    // of the vj and 1-indexing of j to the 0-indexing of the cstring
    nextEDRcolumn(E,R,j0,u,ui,p,v[vj+j-2],j,costFunc);
  }
}

// evaluate crosspoints; simply algorithm from script in c-code
// caution: uses 1-indices for sequences and 0-indices for matrices
// returns the costs of the alignment
int evaluatecrosspoints(int* E, int* R, int* C, char* u, int i, int p, char* v, int j, int q, int (*costFunc)(char,char))
{
  int j0,i0;
  int cost = 0;
  if (q >= 2)
  {
    j0 = q/2;
    evaluateallcolumns(E,R,j0,u,i,p,v,j,q,costFunc);
    i0 = R[p];
    cost = E[p];
    C[j+j0-1] = i+i0-1;
    evaluatecrosspoints(E,R,C,u,i,i0,v,j,j0,costFunc);
    evaluatecrosspoints(E,R,C,u,i+i0,p-i0,v,j+j0,q-j0,costFunc);
  }
  return cost;
}

/* 
   construct alignment from crosspoint-table C
   PROBLEM: some information for the reconstruction may be
   missing in the crosspoint table (because there can be
   more than one minimizing points in one column;
   IDEA: recalculate in case of changing row index the alignment
   between both crosspoints: because the column-index always increases
   by one (per definition) it is the alignment of a single character v[j]
   against the substring u[C[i-1]..C[i]]; this can be done in linear
   space (E and R can be reused).
   and it can be done in linear time because in worst case
   the alignment is recalculated between all crosspoints, which
   costs (m*1) time;
   the overall performance of the traceback is O(m+n)
   (as it is in the "classical" non-linear-space algorithm)
*/
alignment* construct_alignment(int* C,int* E, int* R, char*u,int m,char* v, int n,int (*costFunc)(char,char))
{
  int j = n;
  int i,temp,a,b,c,cost;
  char vci;
  
  // determine insertion and deletion costs from cost function
  int in = costFunc('x','-');
  int del = costFunc('-','x');

  // create new alignment
  alignment* alignout = alignment_new(u,m,v,n);

  while (j > 0)
  {
    // if row index changes
    if (C[j] != C[j-1])
    {
        // realign columns from C[j],j to C[j-1],j-1
        E[0] = in;
        // R now holds the direction
        R[0] = 'l';
        vci = v[j-1];

        // fill dynamic programming "matrix"
        // O(m) after accumulation over all iterations of while-loop
        for(i = 1; i <= C[j]-C[j-1]; i++)
        {
          // the first column (epsilon) is represented
          // by i
          a = (i-1)*del + costFunc(u[i+C[j-1]+1-2], vci);
          b = i*del + in;
          c = E[i-1] + del;

          cost = MIN(a,MIN(b,c));

          if (cost == a)
            R[i] = 'd';
          else if (cost == c)
            R[i] = 'o';
          else
            R[i] = 'l';
          E[i] = cost;
        }

        // and now the traceback
        i = C[j]-C[j-1];
        while (i >= 0)
        {
          // after one replacement the rest have to be
          // deletions => end traceback
          if (R[i] == 'd')
          {
            alignment_add_replacement(alignout);
            alignment_add_deletions(alignout,i-1);
            i = -1;
          }
          // and after one insertion the rest are deletions
          // => end traceback
          else if (R[i] == 'l')
          {
            alignment_add_insertion(alignout);
            alignment_add_deletions(alignout,i);
            i = -1;
          }
          // or there are just deletions
          else
          {
            alignment_add_deletion(alignout);
            i--;
          }
        }
    }
    // if row index is not changing => add insertion
    else
    {
      alignment_add_insertion(alignout);
    }
    j--;
  }

  // reverse alignment (because of reverse reconstruction)
  alignment_reverse(alignout);

  return alignout;
}

// perform linear space alignment between two sequences s1 and s2
// with cost function costFunc;
// returns alignment and saves costs to variable at costs
alignment* linear_space_alignment(char* u, char* v, \
    int (*costFunc)(char,char), int* costs)
{
  int m = strlen(u);
  int n = strlen(v);
 
  // allocate memory
  int* E = (int*) malloc((m+1)*sizeof(int));
  int* R = (int*) malloc((m+1)*sizeof(int));

  int* C = (int*) malloc((n+1)*sizeof(int));
  C[0] = 0;
  C[n] = m;

  // calculate the crosspoint-table C
  *costs = evaluatecrosspoints(E,R,C,u,1,m,v,1,n,costFunc);

  // perform traceback
  alignment* a = construct_alignment(C,E,R,u,m,v,n,costFunc);

  /*
  // print crosspoint table C
  int i;
  for (i = 0; i <= n; i++)
  {
    printf("%d,%d;", i,C[i]);
  }
  printf("\n");
  */

  free (E);
  free (R);
  free (C);

  return a;
}

// unit cost function
int unity (char a, char b)
{
  if (a == '-' || b == '-')
    return 1;
  return (a != b);
}

#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    printf("usage: %s seq1 seq2\n", argv[0]);
    return 0;
  }

  char* seq1 = argv[1];//"agtgcacgt";
  char* seq2 = argv[2];//"agcac";

  int costs;
  alignment* a = linear_space_alignment(seq1,seq2,unity,&costs);
  alignment_show (a);
  alignment_delete(a);

  return 0;
}
#endif
