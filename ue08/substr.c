#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* search substrings of length |S| in sequence represented as
   numbers; idea: keep track of differences between number of
   chars in S and substring; a different array is initialized
   with 0 an initial differences to S are counted (parallel |S|
   is determindes); while iterating over string only the
   contributions of the new character at the end and the
   removed character at the end are;
   running time O(n+s) where s is number of different characters
   in S
*/
void substring(char* sequence, int lenseq, int* menge, int lenm)
{
  int* substrdist = (int*) malloc(lenm*sizeof(int));
  int i;
  char c;
  int diff = 0;
  int substrlen = 0;

  for(i = 0; i < lenm; i++)
  {
    substrdist[i] = 0;
    substrlen += menge[i];
    if (menge[i] != 0)
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
    if (menge[c] == substrdist[c])
      diff++;
    else if (menge[c] == substrdist[c]+1)
      diff--;
    substrdist[c]++;
    if (i >= substrlen)
    {
      c = tolower(sequence[i-substrlen])-'a';
      if (menge[c] == substrdist[c])
        diff++;
      else if (menge[c] == substrdist[c]-1)
        diff--;
      substrdist[c]--;
    }

    if (i >= substrlen-1 && diff == 0)
    {
      printf("matching substring found starting at %d (length %d)\n", i-substrlen+2, substrlen);
    }
  }

  free(substrdist);
    
}


int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    printf("usage: %s sequence menge(format a=0 c=3 ...)\n", argv[0]);
    return 0;
  }

  char* seq = argv[1];
  int* menge = (int*) calloc(26, sizeof(int));
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
    menge[c-'a'] = num;
  }

  substring(seq, lenseq, menge, 26);
  
  free(menge);
  return 0;
}
