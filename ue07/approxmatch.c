/* 
   determine the approximate string matches for two strings;
   usage: argv[0] error-threshold pattern text
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN(a,b) (a < b ? a : b)

int unity(char a, char b)
{
  if (a == '\001' || b == '\001')
    return 1;
  if (a == b)
    return 0;
  return 1;
}

/* 
   idea for start-position: save in a second (|p|+1)-vector the length of the
   match; if minimizing edge is from northwest or west
      => match is extended by one;
   the current match length is calculated parallel to the costs
   (at the same position in its own array) => O(|p|) space;
*/
void approxsearch(int k, char* p, int plen, char* t, int tlen, int (*costFunc) (char, char))
{
  int i,j,oldij,oldlen,len;
  int a,b,c,min;
  int newlen = 0;
  int gap = costFunc('\001','\001');
  
  // initialize vectors and match
  int* vec = (int *) malloc((plen+1)*sizeof(int));
  int* lenvec = (int *) calloc((plen+1), sizeof(int));
  char* match = (char *) malloc((tlen+1)*sizeof(char));

  vec[0] = 0; // initialize 0,0
  // all columns
  for (j = 0; j <= tlen; j++)
  {
    // each match starts at 0 (with length 0)
    oldij = 0;
    oldlen = 0;
    // all rows >= 1 (0 is always 0)
    for (i = 1; i <= plen; i++)
    {
      if (j == 0)
      {
        // recursion for j == 0
        vec[i] = vec[i-1] + gap;
      }
      else
      {
        // recursion for j != 0
        a = vec[i-1] + gap;
        b = vec[i] + gap;
        c = oldij + costFunc(p[i-1],t[j-1]);
        min = MIN(MIN(a,b),c);

        // direction for length calculation
        if (min == c)
          newlen = oldlen + 1;
        else if (min == b)
          newlen = lenvec[i] + 1;
        else if (min == a)
          newlen = lenvec[i-1];
        
        // save vec[i] and lenvec[i] before overwriting (as i-1, j-1)
        oldij = vec[i];
        oldlen = lenvec[i];

        lenvec[i] = newlen;
        vec[i] = min;
      }
    }
    // in last row => we find potential matches
    if (vec[i-1] <= k)
    {
      // a match ends at j-1
      len = lenvec[i-1];
      // copy matching string ...
      strncpy(match,&t[j-1-len+1],len);
      match[len] = '\0';
      // ... and print results
      printf("match ends at %d with length %d: %s matches %s\n",j, lenvec[i-1], match, p);
    }
  }

  free (vec);
  free (lenvec);
}

int main(int argc, char* argv[])
{
  // parse input arguments

  if (argc < 4)
  {
    printf("usage: %s k(error) p(pattern) t(text)\n", argv[0]);
    return 0;
  }

  int k = -1;
  sscanf(argv[1], "%d", &k);
  if (k < 0)
  {
    printf("k may not be negative\n");
    return 1;
  }
  char* p = argv[2];
  int plen = strlen(p);
  char* t = argv[3];
  int tlen = strlen(t);

  approxsearch(k,p,plen,t,tlen,unity);

  return 0;
}
