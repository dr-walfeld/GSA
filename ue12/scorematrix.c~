/* representation of a score matrix (i.e. BLOSUM) for sequence alignment;
   score matrix can be read from file and can be accessed with
   score (char a, char b) to get the scoring value of a and b;
   the score function and storage of matrix is case insensitive
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scorematrix.h"

#define INF 1000000
#define LINE 1024

scorematrix* initialize_scorematrix (int m, int indel)
{
  int i,j;
  scorematrix* sm = (scorematrix*) malloc(sizeof(scorematrix));
  int** matrix = (int**) malloc(m*sizeof(int*));
  int* row;

  // reserve memory as one block
  row = (int*) malloc(m*m*sizeof(int));
  for (i = 0; i < m; i++)
  {
    matrix[i] = &row[i*m];
    for (j = 0; j < m; j++)
    {
      matrix[i][j] = INF;
    }
  }

  sm->matrix = matrix;
  sm->indel = indel;
  return sm;
}

void delete_scorematrix(scorematrix* sm)
{
  if (sm == NULL)
    return;

  // matrix[0] contains pointer to reserved memory block
  free(sm->matrix[0]);
  free(sm->matrix);
  free(sm);
}

int get_score (scorematrix* sm, char a, char b)
{
  if (a == '-' || b == '-')
    return sm->indel;

  a = toupper(a);
  b = toupper(b);

  if (a == '*')
    a = '[';
  if (b == '*')
    b = '[';

  int score;

  if (!isupper(a) && a != '[')
  {
    printf("ERROR: %c is not an allowed character!\n",a);
    return INF;
  }
  if (!isupper(b) && b != '[')
  {
    printf("ERROR: %c is not an allowed character!\n",b);
    return INF;
  }

  score = sm->matrix[a-'A'][b-'A'];
  if (score == INF)
  {
    ;//printf("ERROR: exchange from %c and %c is not defined!\n", a, b);
  }
  return score;
}

// read scorematrix from file
scorematrix* read_scorematrix (char* filename, int indel)
{
  char ch;
  char line[LINE];
  // open file
  FILE * filestream = fopen (filename, "rt");
  char *wordptr;
  int score;
  int i = 0;
  int len = 0;
  int linecount = 0;

  int firstline = 1;

  // if opening of file failes
  if (filestream == NULL)
  {
    printf("ERROR: could not open file %s!\n", filename);
    return NULL;
  }

  // initialize allscorematrix (with A-Z) und *
  // * will be replaced by [ (following Z)
  scorematrix* sm = initialize_scorematrix (27,indel);
  int position[27];
  int usedcol[27];
  int usedrow[27];

  for (i = 0; i < 27; i++)
  {
    position[i] = -1;
    usedcol[i] = 0;
    usedrow[i] = 0;
  }

  // read lines
  while (fgets(line, LINE, filestream) != NULL)
  {
    i = 0;
    // empty lines and commentary # are ignored
    if (line[0] != '\n' && line[0] != '#')
    {
      line[strlen(line)-1] = '\0'; // remove trailing \n
      // split line at blank
      wordptr = strtok(line, " ");
      while (wordptr != NULL)
      {
        // if firstline
        if (firstline)
        {
          // look for characters
          if (sscanf(wordptr, "%c", &ch) == 0)
          {
            puts("ERROR: no character found");
            fclose (filestream);
            return NULL;
          }
          // only uppercase or lowercase letters are isupper after toupper
          ch = toupper(ch);
          if (isupper(ch) || ch == '*')
          {
            if (ch == '*')
              ch = '[';
            // check if character has been used
            if (usedcol[ch-'A'])
            {
              printf("ERROR: symbol %c used more than once in columns\n", ch);
              fclose (filestream);
              return NULL;
            }
            position[i] = ch;
            usedcol[ch-'A'] = 1; // to determine used chars
          }
          else
          {
            printf("ERROR: symbol %c is not allowed\n", ch);
            fclose (filestream);
            return NULL;
          }
        }
        // if not firstline
        else
        {
          // first field of each line is character
          if (i == 0)
          {
            // if no character
            if(sscanf(wordptr, "%c", &ch) == 0)
            {
              puts("ERROR: no character found");
              fclose (filestream);
              return NULL;
            }
            ch = toupper(ch);
           
            if (isupper(ch) || ch == '*')
            { 
              if (ch == '*')
                ch = '[';
              // check if entry is unset
              if (!usedcol[ch-'A'])
              {
                printf("ERROR: symbol %c is used in row dimension only\n", ch);
                fclose (filestream);
                return NULL;
              }

              // check if entry in first column is set (=> double symbol)
              if (usedrow[ch-'A'])
              {
                printf("ERROR: symbol %c used more than once in rows\n", ch);
                fclose (filestream);
                return NULL;
              }
              usedrow[ch-'A'] = 1;
              linecount += 1;
            }
            else 
            {
              printf("ERROR: symbol %c is not allowed\n", ch);
              fclose (filestream);
              return NULL;
            }

          }
          // if not first character
          else
          {
            if (sscanf(wordptr, "%d", &score) == 0)
            {
              puts("ERROR: no score value found");
              fclose (filestream);
              return NULL;
            }

            if (i-1 >= len)
            {
              puts("ERROR: more entries than allowed detected");
              fclose (filestream);
              return NULL;
            }

            // it has been already checked that
            // ch and position[i-1] are valid characters
            // and therefore exist in the matrix
            sm->matrix[ch-'A'][position[i-1]-'A'] = score;
            if (sm->matrix[position[i-1]-'A'][ch-'A'] != INF && sm->matrix[position[i-1]-'A'][ch-'A'] != score)
            {
              printf("WARNING: asymmetric matrix detected, different entries for (%c,%c) and (%c,%c)\n", ch, position[i-1], position[i-1], ch);
            }
          }
        }

        // read next field
        wordptr = strtok(NULL, " ");
        i++;
      }
      if (!firstline && i-1 != len)
      {
        puts("ERROR: different number of entries in lines detected!");
        fclose (filestream);
        return NULL;
      }
      if (firstline)
      {
        firstline = 0; // the first line has been read
        len = i; 
      }
    }
  }

  /* because there are no double entries in rows and rows
     contain only entriey present in columns => row count
     must match line count */
  if (linecount != len)
  {
    printf("ERROR: a symbol is used in column dimension only\n");
    fclose (filestream);
    return NULL;
  }

  return sm;
}

void show_scorematrix(scorematrix* sm)
{
  int i,j,print;
  for (i = 'A'-1; i <= '['; i++)
  {
    print = 0;
    for (j = 'A'-1; j <= '['; j++)
    {
      if (i == 'A'-1 && j == 'A'-1)
      {
        printf("  ");
      }
      else if (i == 'A'-1 && get_score(sm,j,'A') < INF)
      {
        printf(" %c ", j);
        print = 1;
      }
      else if (j == 'A'-1 && get_score(sm,i,'A') < INF)
      {
        printf("%c ", i);
        print = 1;
      }
      else if (i >= 'A' && j >= 'A' && get_score(sm,i,j) < INF)
        printf("%2d ", get_score(sm,i,j));
      else
        ;//printf("   ");
    }
    if (print)
      printf("\n");
  }

}

#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    puts("ERROR: no file and no indel specified");
    return 1;
  }

  int indel;
  sscanf(argv[2], "%d", &indel);
  scorematrix* sm = read_scorematrix(argv[1],indel);

  if (sm == NULL)
    return 1;

  show_scorematrix(sm);

  delete_scorematrix(sm);

  return 0;
}
#endif
