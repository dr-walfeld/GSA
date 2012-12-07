#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* search substrings of length |S| in sequence represented as
   numbers; idea: keep track of differences between number of
   chars in S and substring; a different array is initialized
   with 0 an initial differences to S are counted (parallel |S|
   is determined); while iterating over string only the
   contributions of the new character at the end and the
   removed character at the end arei considered;
   running time O(n+s) where s is number of different characters
   in S
*/
void substring(char* sequence, int lenseq, int* set, int lenset)
{
  int* substrdist = (int*) malloc(lenset*sizeof(int));
  int i;
  int c;
  int diff = 0;
  int substrlen = 0;

  for(i = 0; i < lenset; i++)
  {
    substrdist[i] = 0;
    substrlen += set[i];
    if (set[i] != 0)
      diff++;
  }

  for(i = 0; i < lenseq; i++)
  {
    c = tolower(sequence[i]);
    if (!islower(c))
    {
      printf("%c is no allowed character!\n", c);
      return;
    }
    c -= 'a';
    if (set[c] == substrdist[c])
      diff++;
    else if (set[c] == substrdist[c]+1)
      diff--;
    substrdist[c]++;
    if (i >= substrlen)
    {
      c = tolower(sequence[i-substrlen])-'a';
      if (set[c] == substrdist[c])
        diff++;
      else if (set[c] == substrdist[c]-1)
        diff--;
      substrdist[c]--;
    }

    if (i >= substrlen-1 && diff == 0)
    {
      printf("matching substring found starting at %d (length %d)\n", \
          i-substrlen+1, substrlen);
    }
  }

  free(substrdist);
    
}


int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    printf("usage: %s sequence set(format a=0 c=3 ...)\n", argv[0]);
    return 0;
  }

  char* seq = argv[1];
  int lenset = 26;
  int* set = (int*) calloc(lenset, sizeof(int));
  int lenseq = strlen(seq);
  int i;

  char c;
  int num;
  for (i = 2; i < argc; i++)
  {
    c = 0;
    num = -1;
    sscanf(argv[i], "%c=%d", &c, &num);
    c = tolower(c);
    if (!islower(c) || num == -1)
    {
      printf("expression %s has incorrect format!\n", argv[i]);
      return 1;
    }
    set[c-'a'] = num;
  }

  substring(seq, lenseq, set, lenset);
  
  free(set);
  return 0;
}
