#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fasta.h"

#define MAX_LINE 1024

int isBase(char);

int isBase(char c)
{
  switch (c)
  {
    case 'A':
      return 1;
    case 'a':
      return 1;
    case 'C':
      return 1;
    case 'c':
      return 1;
    case 'G':
      return 1;
    case 'g':
      return 1;
    case 'T':
      return 1;
    case 't':
      return 1;
    case 'U':
      return 1;
    case 'u':
      return 1;
    default:
      return 0;
  }
}

fasta* fasta_new (int csize)
{
  fasta* temp = (fasta *) malloc (sizeof(fasta));
  temp->length = 0;
  temp->clength = 0;
  temp->header = (char *) malloc (csize*sizeof(char));
  temp->sequence = (char *) malloc (csize*sizeof(char));
  temp->csize = csize;
  temp->ssize = csize;
  return temp;
}

void fasta_delete (fasta* f)
{
  free (f->header);
  free (f->sequence);
  free (f);
}

void fasta_add_base (fasta* f, char base)
{
  if (f->length == f->ssize)
  {
    f->ssize *= 2;
    f->sequence = (char *) realloc (f->sequence, sizeof(char)*f->ssize);
  }
  f->sequence[f->length] = base;
  // 0-terminator does not elong string
  if (base != '\0')
    f->length++;
}

void fasta_add_header_char (fasta* f, char ch)
{
  if (f->clength == f->csize)
  {
    f->csize *= 2;
    f->header = (char *) realloc (f->header, sizeof(char)*f->csize);
  }
  f->header[f->clength] = ch;
  // the 0-terminator does not contribute to the string length
  // and may (and should) be overwritten everytime
  if (ch != '\0')
    f->clength++;
}

multifasta* multifasta_new(int size)
{
  multifasta* temp = (multifasta *) malloc (sizeof(multifasta));
  temp->length = 0;
  temp->maxlen = size;
  temp->entries = (fasta **) malloc(size*sizeof(fasta*));

  return temp;
}

void multifasta_delete (multifasta* mf)
{
  int i;
  for (i = 0; i < mf->length; i++)
  {
    fasta_delete(mf->entries[i]);
  }
  free (mf->entries);
  free (mf);
}

void multifasta_add_fasta (multifasta * mf, fasta * f)
{
  if (mf->length == mf->maxlen)
  {
    mf->maxlen *= 2;
    mf->entries = (fasta **) realloc (mf->entries, sizeof(fasta*)*mf->maxlen);
  }
  mf->entries[mf->length] = f;
  mf->length++;
}


multifasta* read_fasta_file (char* filename)
{
  char ch;

  FILE *filestream = fopen(filename, "rt");
  
  if (filestream == NULL)
  {
    printf("FEHLER: Datei %s konnte nicht geöffnet werden!\n", filename);
    return NULL;
  }

  multifasta* mfast = multifasta_new(20);

  fasta* seq = NULL;
  
  int newline = 1;
  int header = 0;
  int sequence = 0;
  int comment = 0;
  do
  {
    // read current char
    ch = fgetc (filestream);
    // check if line just started
    if (newline)
    {
      // check if its just another newline
      newline = (ch == '\n');
      // check if it is a > (header starts)
      if (ch == '>')
      {
        // if header => must not happen
        if (header)
        {
          puts ("FEHLER: Datei ist nicht FASTA-Format (Header darf nicht auf Header folgen)!");
          fclose(filestream);
          multifasta_delete (mfast);
          return NULL; // zwei header hintereinander sind nicht erlaubt
        }
        // now the header follows
        header = 1;
        // create new fasta struct
        if (sequence)
        {
          multifasta_add_fasta (mfast, seq);
          fasta_add_base(seq, '\0');
          sequence = 0;// finish sequence
        }
        seq = fasta_new (MAX_LINE);
       
      }
      // if comment line is starting
      else if (ch == '#')
      {
        comment = 1;
      }
      // if a base is the first character
      else if (isBase(ch))
      {
        // if already a sequence is read => just add base
        if (sequence)
        {
          fasta_add_base(seq, ch);
        }
        // else start sequence
        else
        {
          fasta_add_base(seq, ch);
          sequence = 1;
          // if no header preceeded => then it's not FASTA
          if (!header)
          {
            puts("FEHLER: Datei ist nicht FASTA (Header fehlt)!");
            fclose(filestream);
            multifasta_delete (mfast);
            return NULL;
          }
          header = 0;
        }
      }
    }
    // if we didn't start a newline
    else
    {
      // check if its a newline character
      newline = (ch == '\n');
      // check if comment and if its a newline => now comment ends
      // all other chars are ignored (because its a comment)
      if (comment)
      {
        if (newline)
        {
          comment = 0;
        }
      }
      if (header)
      {
        // if there is no newline => add it to sequence header
        if (!newline)
          fasta_add_header_char(seq, ch);
        else
          fasta_add_header_char(seq, '\0');
      }
      // thats ok because after a sequence started, there is never (header)
      else if (sequence)
      {
        if (isBase(ch))
          fasta_add_base(seq, ch);
      }
    }

    //printf("%s", line);
  } while (ch != EOF);

  if (header || !sequence)
  {
    puts ("FEHLER: Datei endet nicht mit Sequenz!");
    fclose(filestream);
    multifasta_delete (mfast);
    return NULL;
  }

  // finish last added sequence
  multifasta_add_fasta (mfast, seq);
  fasta_add_base(seq, '\0');

  fclose(filestream);

  return mfast;
}

#ifdef UNIT_TEST

int main(int argc, char * argv[])
{
  multifasta *mf = read_fasta_file (argv[1]);

  if (mf == NULL)
    return 1;
  
  int i;
  for (i = 0; i < mf->length; i++)
  {
    printf("Entry %d \n", i);
    printf("Header: %s \n", mf->entries[i]->header);
    printf("Sequenz: %s \n", mf->entries[i]->sequence);
  }
  multifasta_delete (mf);
  return 0;
}
#endif
