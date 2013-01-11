#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fasta.h"

#define MAXLINE 1024
#define STARTSIZE 16

int** read_hits (char* filename, int* length)
{
  *length = 0;
  int reserved = STARTSIZE;
  int** output = (int**) malloc(STARTSIZE*sizeof(int*));
  FILE* filestream = fopen(filename,"rt");
  if (filestream == NULL)
  {
    printf("ERROR: could not open file %s!\n", filename);
    return NULL;
  }

  char line[MAXLINE];
  while (fgets(line,MAXLINE,filestream) != NULL)
  {
    /* skip newlines */
    if (line[0] == '\n')
      continue;

    int e1,e2;
    if (sscanf(line,"[%d,%d]",&e1,&e2) != 2)
    {
      fclose(filestream);
      printf("ERROR: incorrect line format for line %s!\n", line);
      return NULL;
    }
    
    if (*length == reserved)
    {
      reserved *= 2;
      output = realloc(output, sizeof(int*)*reserved);
    }
    output[*length] = malloc(2*sizeof(int));
    output[*length][0] = e1;
    output[*length][1] = e2;
    (*length)++;
  }

  fclose(filestream);

  return output;
}

int extendhitleft(char* u,int m,char* w,int n,int q,int match,int mismatch,int xd,int i,int j)
{
  int xmax = 0;
  int scoresum = 0;
  int l = 0;
  int sc;
  while (i-1-l >= 0 && j-1-l >= 0)
  {
    if (u[i-1-l] == w[j-1-l])
      sc = match;
    else
      sc = mismatch;
      printf("%c", u[i-1-l]);

    scoresum += sc;

    if (scoresum < (xmax-xd))
      break;
    if (scoresum > xmax)
      xmax = scoresum;

    l++;
  }
  printf("\n");
  return l-1;
}

int extendhitright(char* u,int m,char* w,int n,int q,int match,int mismatch,int xd,int i,int j)
{
  /*printf("control: match = %d, mismatch = %d, q = %d, xd = %d\n", match,
      mismatch, q, xd);*/
  int xmax = 0;
  int scoresum = 0;
  int r = 0;
  int sc;
  while (i+q+r < m && j+q+r < n)
  {
    if (u[i+q+r] == w[j+q+r])
      sc = match;
    else
      sc = mismatch;

    printf("%c",u[i+q+r]);

    scoresum += sc;

    if (scoresum < (xmax-xd))
    {
      break;
    }
    if (scoresum > xmax)
    {
      xmax = scoresum;
    }
    r++;
  }
  printf("\n");
  return r-1;
}

int main(int argc, char* argv[])
{
  if (argc < 8)
  {
    printf("usage: %s query.fasta database.fasta blasthits q match mismatch xd\n",
        argv[0]);
    return 0;
  }

  int q,match,mismatch,xd;
  if (sscanf(argv[4],"%d",&q) != 1 || q <= 0)
  {
    printf("%s is no valid positive integer for q!\n", argv[4]);
    return 1;
  }
  if (sscanf(argv[5],"%d",&match) != 1)
  {
    printf("%s is no valid integer for match!\n", argv[5]);
    return 1;
  }
  if (sscanf(argv[6],"%d",&mismatch) != 1)
  {
    printf("%s is no valid integer for mismatch!\n", argv[6]);
    return 1;
  }
  if (sscanf(argv[7],"%d",&xd) != 1 || xd <= 0)
  {
    printf("%s is no valid positive integer for xd!\n", argv[7]);
    return 1;
  }

  multifasta* mf1 = read_fasta_file(argv[1]);
  multifasta* mf2 = read_fasta_file(argv[2]);

  if (mf1 == NULL || mf2 == NULL)
    return 1;

  char* query = mf1->entries[0]->sequence;
  char* database = mf2->entries[0]->sequence;
  int m = strlen(database);
  int n = strlen(query);

  int length;
  int** hits = read_hits(argv[3],&length);
  if (hits == NULL)
    return 1;

  int i,right,left,leftbd,leftbq,rightbd,rightbq,len;
  for (i = 0; i < length; i++)
  {
    left =
      extendhitleft(database,m,query,n,q,match,mismatch,xd,hits[i][0],hits[i][1]);
    right =
      extendhitright(database,m,query,n,q,match,mismatch,xd,hits[i][0],hits[i][1]);
    leftbd = hits[i][0]-1-left;
    leftbq = hits[i][1]-1-left;
    rightbd = hits[i][0]+right+q;
    rightbq = hits[i][1]+right+q;
    len = rightbd-leftbd+1;
    printf("(%d-%d) (%d-%d) length %d\n",leftbd,rightbd,leftbq,rightbq,len);
    char* print = &(database[leftbd]);
    char temp = database[rightbd+1];
    database[rightbd+1] = '\0';
    printf("%s\n",print);
    database[rightbd+1] = temp;
    print = &(query[leftbq]);
    temp = query[rightbq+1];
    query[rightbq+1] = '\0';
    printf("%s\n",print);
    query[rightbq+1] = temp;


    printf("\n");
  }

  //printf("\n %c",database[333]);
  free(hits);
  multifasta_delete(mf1);
  multifasta_delete(mf2);
  return 0;
}
