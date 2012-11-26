#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INF 1000000

int get_weight (char c)
{
  switch (c)
  {
    case 'a':
      return 1;
    case 't':
      return 2;
    case 'g':
      return 3;
    case 'c':
      return 2;
    default:
      return INF;
  }
}

/* find weighted substrings of a sequence with length N in O(N)
   requirement: weights are always positive integer so the
   addition of an element ALWAYS increases the weight and the
   deletion of an element ALWAYS decreases the weight
   => one can simply walk over the sequence and add the next
      element if the weight is smaller than the target and
      remove the first element of the substring if the weight
      is bigger than the target 
*/
void find_substrings (char* sequence, int len, int tweight, int (*weightFunc) (char))
{
  int r = 0;
  int l = 0;
  int weight = 0;

  while (r <= len)
  {
    if (weight < tweight)
    {
      if (r < len)
        weight += weightFunc(sequence[r]);
      r++;
    }
    else if (weight > tweight)
    {
      weight -= weightFunc(sequence[l]);
      l++;
    }
    else if (weight == tweight) 
    {
      // a little bit tricky: its the incremented l++ (because
      // l is no more part of the substring) but r (so
      // length is r-1-l+1) because r is part of the substring
      printf("(%d,%d)\n", l+1, r-l);
      // always positive integers so r+1 OR l+1 is never a match
      weight += weightFunc(sequence[r]);
      weight -= weightFunc(sequence[l]);
      r++;
      l++;
    }
  }
}

int main (int argc, char * argv[])
{
  if (argc < 3)
  {
    printf("usage: %s sequence weight\n", argv[0]);
    return 0;
  }

  char* seq = argv[1];
  int weight;
  int len = strlen(seq);
  if(sscanf(argv[2],"%d",&weight) == 0)
  {
    printf("ERROR: %s is no valid integer value!\n", argv[2]);
    return 1;
  }

  find_substrings (seq, len, weight, get_weight);

  return 0;
}
