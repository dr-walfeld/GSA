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

int** initialize_scorematrix (int m)
{
  int i,j;
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

  return matrix;
}

void delete_scorematrix(int** matrix)
{
  if (matrix == NULL)
    return;

  // matrix[0] contains pointer to reserved memory block
  free(matrix[0]);
  free(matrix);
}

int score (int** matrix, char a, char b)
{
  a = toupper(a);
  b = toupper(b);
  int score;

  if (!isupper(a))
  {
    printf("ERROR: %c is not an allowed character!\n",a);
    return INF;
  }
  if (!isupper(b))
  {
    printf("ERROR: %c is not an allowed character!\n",b);
    return INF;
  }

  score = matrix[a-'A'][b-'A'];
  if (score == INF)
  {
    ;//printf("ERROR: exchange from %c and %c is not defined!\n", a, b);
  }
  return score;
}

// read scorematrix from file
int** read_scorematrix (char* filename)
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

  // initialize allscorematrix (with A-Z)
  int** matrix = initialize_scorematrix (26);
  int position[26];
  int usedcol[26];
  int usedrow[26];

  for (i = 0; i < 26; i++)
  {
    position[i] = -1;
    usedcol[i] = 0;
    usedrow[i] = 0;
  }

  // read lines
  while (fgets(line, LINE, filestream) != NULL)
  {
    i = 0;
    // empty lines are ignored
    if (line[0] != '\n')
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
            return NULL;
          }
          // only uppercase or lowercase letters are isupper after toupper
          ch = toupper(ch);
          if (!isupper(ch))
          {
            printf("ERROR: symbol %c is not allowed\n", ch);
            return NULL;
          }
          // check if character has been used
          if (usedcol[ch-'A'])
          {
            printf("ERROR: symbol %c used more than once in columns\n", ch);
            return NULL;
          }
          position[i] = ch;
          usedcol[ch-'A'] = 1; // to determine used chars
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
              return NULL;
            }
            ch = toupper(ch);
            if (!isupper(ch))
            {
              printf("ERROR: symbol %c is not allowed\n", ch);
              return NULL;
            }
            
            // check if entry is unset
            if (!usedcol[ch-'A'])
            {
              printf("ERROR: symbol %c is used in row dimension only\n", ch);
              return NULL;
            }

            // check if entry in first column is set (=> double symbol)
            if (usedrow[ch-'A'])
            {
              printf("ERROR: symbol %c used more than once in rows\n", ch);
              return NULL;
            }
            usedrow[ch-'A'] = 1;
            linecount += 1;
          }
          // if not first character
          else
          {
            if (sscanf(wordptr, "%d", &score) == 0)
            {
              puts("ERROR: no score value found");
              return NULL;
            }

            if (i-1 >= len)
            {
              puts("ERROR: more entries than allowed detected");
              return NULL;
            }

            // it has been already checked that
            // ch and position[i-1] are valid characters
            // and therefore exist in the matrix
            matrix[ch-'A'][position[i-1]-'A'] = score;
            if (matrix[position[i-1]-'A'][ch-'A'] != INF && matrix[position[i-1]-'A'][ch-'A'] != score)
            {
              printf("WARNING: asymmetric matrix detected, different entries for (%c,%c) and (%c,%c)\n", ch, position[i-1], position[i-1], ch);
            }
          }
        }

        // read nect field
        wordptr = strtok(NULL, " ");
        i++;
      }
      if (!firstline && i-1 != len)
      {
        puts("ERROR: different number of entries in lines detected!");
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
    return NULL;
  }

  return matrix;
}

#ifdef UNIT_TEST
int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    puts("ERROR: no file specified");
    return 1;
  }

  int** matrix = read_scorematrix(argv[1]);

  if (matrix == NULL)
    return 1;

  int i,j,print;
  for (i = 'A'-1; i <= 'Z'; i++)
  {
    print = 0;
    for (j = 'A'-1; j <= 'Z'; j++)
    {
      if (i == 'A'-1 && j == 'A'-1)
      {
        printf("  ");
      }
      else if (i == 'A'-1 && score(matrix,j,'A') < INF)
      {
        printf(" %c ", j);
        print = 1;
      }
      else if (j == 'A'-1 && score(matrix,i,'A') < INF)
      {
        printf("%c ", i);
        print = 1;
      }
      else if (i >= 'A' && j >= 'A' && score(matrix,i,j) < INF)
        printf("%2d ", score(matrix,i,j));
      else
        ;//printf("   ");
    }
    if (print)
      printf("\n");
  }

  delete_scorematrix(matrix);

  return 0;
}
#endif
