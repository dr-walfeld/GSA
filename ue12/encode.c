#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "encode.h"

/* pre-calculate 0..r^q */
int* get_pot_list(int r, int q)
{
  int* result;
  int i;

  result = (int*) malloc((q+1)*sizeof(int));
  result[0] = 1;
  for (i = 1; i <= q; i++)
  {
    result[i] = result[i-1]*r;
  }

  return result;
}

/* translate character with alphabet A */
inline int translate (int c, int* A, int* fail)
{
  if (A[c] == -1)
  {
    printf("ERROR: character %c is not defined in alphabet!\n", c);
    *fail = 1;
  }
  return A[c];
}

/* translate one qword with precalculated exponentiation list) */
inline int translate_qword(char* w, int q, int* A, int* rpot)
{
  int i,fail=0, c = 0;

  for (i = 1; i <= q; i++)
  {
    c += translate(w[i-1],A,&fail)*rpot[q-i];
    if (fail)
      return -1;
  }

  return c;
 
}

/* translate sequence (cstring) of length n into integercode-array 
  (with alphabet A of length r and wordsize q) of length n-q+1 */
int* translate_sequence(char* w, int n, int q, int r, int* A)
{
  unsigned long int i;
  int *rpot,*output,c = 0,fail = 0;

  if (q > n)
  {
    printf("ERROR: q may not be larger than sequence length\n");
    return NULL;
  }

  /* precalculated exponentiation r^q */
  rpot = get_pot_list(r,q);

  /* reserve memory for output */
  output = (int*) malloc((n-q+1)*sizeof(int));
  assert(output != NULL);

  /* calculate first q word in u and add to list */
  for (i = 1; i <= q; i++)
  {
    c += translate(w[i-1],A,&fail)*rpot[q-i];
    if (fail)
      return NULL;
  }
  output[0] = c;

  /* calculate all subsequent q words in u and mark positions  */
  for (i = 1; i <= n-q; i++)
  {
    c = (c - translate(w[i-1],A,&fail)*rpot[q-1])*r + translate(w[i+q-1],A,&fail);
    if (fail)
      return NULL;
    output[i] = c;
  }

  free(rpot);
  return output;
}

/* create new alphabet from cstring */
int* new_alphabet(char* s)
{
  unsigned int i;
  int len, *a;

  /* reserve memory (for all allowed characters) */
  a = (int*) malloc(UCHAR_MAX*sizeof(int));
  assert(a != NULL);

  len = strlen(s);
  /* initialize with -1 */
  for (i = 0; i < UCHAR_MAX; i++)
  {
    a[i] = -1;
  }
  /* save position of character in alphabet-string as code */
  for (i = 0; i < len; i++)
  {
    /* multiple characters in alphabet are not allowed => hamper encoding by
       position */
    if (a[(int)s[i]] != -1)
    {
      printf("ERROR: %c more than once in alphabet!\n",s[i]);
      return NULL;
    }
    else
    {
      a[(int)s[i]] = i;
    }
  }
  return a;
}

