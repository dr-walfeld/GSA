/*
   representation of position specific scoring matrix
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

#include "pssm.h"

#define LINE 1024

/* reserve memory for PSSM (with characters alphabet characters and length
   positions) */
static Pssm* initialize_pssm (int characters, int positions)
{
  int i,j;
  Pssm* pssm;
  int** matrix;
  int* row;
  int* maxscore;

  pssm = malloc(sizeof(Pssm));
  assert(pssm != NULL);
  matrix = (int**) malloc(characters*sizeof(int*));
  assert(matrix != NULL);

  // reserve memory as one block
  row = (int*) malloc(characters*positions*sizeof(int));
  assert(row != NULL);
  for (i = 0; i < characters; i++)
  {
    matrix[i] = &row[i*positions];
    for (j = 0; j < positions; j++)
    {
      matrix[i][j] = INT_MIN;
    }
  }

  maxscore = malloc(positions*sizeof(int));
  assert(maxscore != NULL);
  pssm->maxscore = maxscore;
  pssm->characters = characters;
  pssm->positions = positions;
  pssm->matrix = matrix;
  return pssm;
}

/* free memory of PSSM */
void delete_pssm(Pssm* pssm)
{
  if (pssm == NULL)
    return;

  // matrix[0] contains pointer to reserved memory block
  free(pssm->matrix[0]);
  free(pssm->matrix);
  free(pssm->maxscore);
  free(pssm);
}

/* calculate maximum score for each position in PSSM */
static void max_pssmscore (Pssm* pssm)
{
  int i,j,sc;

  for (i = 0; i < pssm->positions; i++)
  {
    pssm->maxscore[i] = INT_MIN;
    for (j = 0; j < pssm->characters; j++)
    {
      if ((sc = get_pssmscore(pssm, i, j+'A')) > pssm->maxscore[i])
      {
        pssm->maxscore[i] = sc;
      }
    }
  }
}

/* get maximum score at position d */
inline int get_pssmmaxscore(Pssm* pssm, int d)
{
  if (d >= pssm->positions)
  {
    fprintf(stderr,"ERROR: position %d is out of range!\n", d);
    return INT_MIN;
  }
  return pssm->maxscore[d];
}

/* get score of character b at position d */
inline int get_pssmscore (Pssm* pssm, int d, char b)
{
  int score;

  b = toupper(b);

  /* replace * by [ */
  if (b == '*')
    b = '[';

  /* check position range */
  if (d < 0 || d >= pssm->positions)
  {
    fprintf(stderr, "ERROR: %d out of range!\n", d);
    return INT_MIN;
  }

  /* check validity of character */
  if (!isupper(b) && b != '[')
  {
    fprintf(stderr, "ERROR: %c is not an allowed character!\n",b);
    return INT_MIN;
  }

  score = pssm->matrix[b-'A'][d];
  if (score == INT_MIN)
  {
    ;//fprintf(stderr, "ERROR: exchange from %c and %c is not defined!\n", a, b);
  }
  return score;
}

/* read scorematrix from file */
Pssm* read_pssm (char* filename)
{
  char ch;
  char line[LINE];
  /* open file */
  FILE * filestream;
  char *wordptr;
  int score;
  int i = 0;
  int len = 0;
  int* usedrow;
  Pssm* pssm;

  filestream = fopen (filename, "rt");

  /* if opening of file failes */
  if (filestream == NULL)
  {
    fprintf(stderr,"ERROR: could not open file %s!\n", filename);
    return NULL;
  }

  /* initialize allscorematrix (with A-Z) und *
     * will be replaced by [ (following Z) */
  usedrow = calloc(27,sizeof(int));

  /* read first non empty line to determine length of PSSM */
  while (fgets(line, LINE, filestream) != NULL)
  {

    if (line[0] != '\n' && line[0] != '#')
    {
      line[strlen(line)-1] = '\0'; /* remove trailing \n */
      /* split line at blank */
      wordptr = strtok(line, " ");
      while (wordptr != NULL)
      {
        wordptr = strtok(NULL, " ");
        i++;
      }
      break;
    }
  } 

  len = i-1;
  /* reserve memory for pssm */
  pssm = initialize_pssm (27,len);

  /* reset file to beginning */
  fseek(filestream, 0, SEEK_SET);

  /* read lines */
  while (fgets(line, LINE, filestream) != NULL)
  {
    i = 0;
    /* empty lines and commentary # are ignored */
    if (line[0] != '\n' && line[0] != '#')
    {
      line[strlen(line)-1] = '\0'; /* remove trailing \n */
      /* split line at blank */
      wordptr = strtok(line, " ");
      while (wordptr != NULL)
      {
          /* first field of each line is character */
          if (i == 0)
          {
            /* if no character */
            if(sscanf(wordptr, "%c", &ch) == 0)
            {
              fprintf(stderr,"ERROR: no character found");
              fclose (filestream);
              delete_pssm(pssm);
              free(usedrow);
              return NULL;
            }
            ch = toupper(ch);
           
            if (isupper(ch) || ch == '*')
            { 
              if (ch == '*')
                ch = '[';

              /* check if entry in first column is set (=> double symbol) */
              if (usedrow[ch-'A'])
              {
                fprintf(stderr,"ERROR: symbol %c used more than once in rows\n", ch);
                fclose (filestream);
                free(usedrow);
                delete_pssm(pssm);
                return NULL;
              }
              usedrow[ch-'A'] = 1;
            }
            else 
            {
              fprintf(stderr,"ERROR: symbol %c is not allowed\n", ch);
              fclose (filestream);
              free(usedrow);
              delete_pssm(pssm);
              return NULL;
            }

          }
          /* if not first character */
          else
          {
            if (sscanf(wordptr, "%d", &score) == 0)
            {
              fprintf(stderr,"ERROR: no score value found");
              fclose (filestream);
              free(usedrow);
              delete_pssm(pssm);
              return NULL;
            }

            /* check if number of entries are matching */
            if (i-1 >= len)
            {
              fprintf(stderr,"ERROR: more entries than allowed detected");
              fclose (filestream);
              free(usedrow);
              delete_pssm(pssm);
              return NULL;
            }

            pssm->matrix[ch-'A'][i-1] = score;
          }

        /* read next field */
        wordptr = strtok(NULL, " ");
        i++;
      }
      /* check if number of entries are matching */
      if (i-1 != len)
      {
        fprintf(stderr,"ERROR: different number of entries in lines detected!");
        fclose (filestream);
        free(usedrow);
        delete_pssm(pssm);
        return NULL;
      }
    }
  }

  /* free memory */
  fclose(filestream);
  free(usedrow);
  max_pssmscore(pssm); /* calculate maximum score for each character */

  return pssm;
}

/* show PSSM on stdout */
void show_pssmatrix(Pssm* pssm)
{
  int i,j,print;
  for (i = 'A'; i <= '['; i++)
  {
    print = 0;
    for (j = -1; j < pssm->positions; j++)
    {
      if (j == -1 && get_pssmscore(pssm,0,i) > INT_MIN)
      {
        printf("%c", i);
        print = 1;
      }
      else if (i >= 'A' && j >= 0 && get_pssmscore(pssm,j,i) > INT_MIN)
        printf(" %3d", get_pssmscore(pssm,j,i));
      else
        ;/*printf("   ");*/
    }
    if (print)
      printf("\n");
  }

}

#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr,"ERROR: no file specified");
    return 1;
  }

  Pssm* pssm = read_pssm(argv[1]);

  if (pssm == NULL)
    return 1;

  show_pssmatrix(pssm);

  delete_pssm(pssm);

  return 0;
}
#endif
