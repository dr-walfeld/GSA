/*
   Einlesen von (Multi-)Fasta-Dateien und Speicherung der Sequenzen
   und Header; Compilieren mit Direktive UNIT_TEST für Test
   Aufruf mit FASTA-Dateiname;
   gibt die Sequenzen mit ihren Headern aus
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "fasta.h"

#define MAX_LINE 1024

/* generate new fasta-entry */
static fasta* fasta_new (int csize)
{
  fasta* temp = (fasta *) malloc (sizeof(fasta));
  assert(temp != NULL);
  temp->length = 0; /* length of sequence */
  temp->clength = 0; /* length of header */
  temp->header = (char *) malloc (csize*sizeof(char));
  temp->sequence = (char *) malloc (csize*sizeof(char));
  temp->csize = csize; /* reserved memory for header */
  temp->ssize = csize; /* reserved memory for sequence*/
  return temp;
}

/* free memory of fasta entry */
static void fasta_delete (fasta* f)
{
  free (f->header);
  free (f->sequence);
  free (f);
}

/* add base to sequence in fasta entry */
static void fasta_add_base (fasta* f, char base)
{
  /* reserve more memory (if reserved memory is not sufficient) */
  if (f->length == f->ssize)
  {
    f->ssize *= 2;
    f->sequence = (char *) realloc (f->sequence, sizeof(char)*f->ssize);
    assert(f->sequence != NULL);
  }
  f->sequence[f->length] = base;
  /* 0-terminator does not elong string */
  if (base != '\0')
    f->length++;
}

/* add character to header-string */
static void fasta_add_header_char (fasta* f, char ch)
{
  if (f->clength == f->csize)
  {
    f->csize *= 2;
    f->header = (char *) realloc (f->header, sizeof(char)*f->csize);
    assert(f->header != NULL);
  }
  f->header[f->clength] = ch;
  /* the 0-terminator does not contribute to the string length
     and may (and should) be overwritten everytime */
  if (ch != '\0')
    f->clength++;
}

/* generate new collection of fasta-entries with initial size */
static multifasta* multifasta_new(int size)
{
  multifasta* temp = (multifasta *) malloc (sizeof(multifasta));
  assert(temp != NULL);
  temp->length = 0;
  temp->maxlen = size;
  temp->entries = (fasta **) malloc(size*sizeof(fasta*));
  assert(temp->entries != NULL);

  return temp;
}

/* free memory of multifasta */
void multifasta_delete (multifasta* mf)
{
  if (mf == NULL)
    return;
  int i;
  for (i = 0; i < mf->length; i++)
  {
    fasta_delete(mf->entries[i]);
  }
  free (mf->entries);
  free (mf);
}

/* add new fasta entry */
static void multifasta_add_fasta (multifasta * mf, fasta * f)
{
  if (mf->length == mf->maxlen)
  {
    mf->maxlen *= 2;
    mf->entries = (fasta **) realloc (mf->entries, sizeof(fasta*)*mf->maxlen);
    assert(mf->entries != NULL);
  }
  mf->entries[mf->length] = f;
  mf->length++;
}
 
/* generate multifasta from fasta file */
multifasta* read_fasta_file (char* filename)
{
  char ch;
  int newline = 1,header = 0,sequence = 0,comment = 0;
  FILE *filestream;
  multifasta* mfast;
  fasta* seq;
  
  /* open file */
  filestream = fopen(filename, "rt");

  /* if file could not be opened ... */
  if (filestream == NULL)
  {
    fprintf(stderr,"ERROR: Can not open File %s!\n", filename);
    return NULL;
  }

  /* initialize multifasta with 20 entries */
  mfast = multifasta_new(20);

  seq = NULL;
  
  do
  {
    /* read current char */
    ch = fgetc (filestream);
    /* check if line just started */
    if (newline)
    {
      /* check if its just another newline */
      newline = (ch == '\n');
      /* check if it is a > (header starts) */
      if (ch == '>')
      {
        /* if header => must not happen */
        if (header)
        {
          fprintf (stderr,"ERROR: File is not FASTA (no header after header allowed)!");
          /* free multifasta, close filestream and return NULL */
          fclose(filestream);
          multifasta_delete (mfast);
          return NULL;
        }
        /* now the header follows */
        header = 1;
        /* is sequence preceeded => finish sequence and add to multifasta */
        if (sequence)
        {
          multifasta_add_fasta (mfast, seq);
          fasta_add_base(seq, '\0');
          sequence = 0;
        }
        /* create new fasta struct */
        seq = fasta_new (MAX_LINE);
       
      }
      /* if comment line is starting */
      else if (ch == '#')
      {
        comment = 1;
      }
      /* if a base is the first character */
      else if (isalpha(ch) || ch == '-')
      {
        /* if already a sequence is beeing read => just add base */
        if (sequence)
        {
          fasta_add_base(seq, ch);
        }
        /* else start sequence */
        else
        {
          fasta_add_base(seq, ch);
          sequence = 1;
          /* if no header preceeded => then it's not FASTA */
          if (!header)
          {
            fprintf(stderr,"ERROR: File is not FASTA (hader missing)!");
            /* free multifasta, close filestream and return NULL */
            fclose(filestream);
            multifasta_delete (mfast);
            return NULL;
          }
          header = 0;
        }
      }
      /* if no base character within sequence; 
         empty lines are ok and file may also end */
      else if (!newline && ch != EOF)
      {
        /* free multifasta, close filestream and return NULL */
        fclose(filestream);
        fprintf(stderr,"ERROR: non-fasta conform line detected! %c is not an allowed symbol!\n", ch);
        multifasta_delete (mfast);
        return NULL;
      }
    }
    /* if we didn't start a newline */
    else
    {
      /* check if its a newline character */
      newline = (ch == '\n');
      /* check if comment and if its a newline => now comment ends
         all other chars are ignored (because its a comment) */
      if (comment)
      {
        if (newline)
        {
          comment = 0;
        }
      }
      /* if reading a header */
      else if (header)
      {
        /* if there is no newline => add it to sequence header */
        if (!newline)
          fasta_add_header_char(seq, ch);
        else
          fasta_add_header_char(seq, '\0');
        /* do NOT set header = 0 (need for checking correct FASTA) */
      }
      /* thats ok because after a sequence started, there is never (header) */
      else if (sequence)
      {
        if (isalpha(ch) || ch == '-')
          fasta_add_base(seq, ch);
        else if (!newline && ch != EOF)
        {
          /* free multifasta, close filestream and return NULL */
          fclose(filestream);
          fprintf(stderr,"ERROR: non-fasta conform line detected! %c is not an allowed symbol!\n",ch);
          multifasta_delete (mfast);
          return NULL;
        }
      }
    }
  } while (ch != EOF); /* stop if EOF is reached */

  /* file may not end with header or without seuqence */
  if (header || !sequence)
  {
    fprintf (stderr,"ERROR: File is not FASTA (sequence missing)!");
    fclose(filestream);
    multifasta_delete (mfast);
    return NULL;
  }

  /* finish last added sequence */
  multifasta_add_fasta (mfast, seq);
  fasta_add_base(seq, '\0');

  fclose(filestream);

  return mfast;
}

#ifdef UNIT_TEST

/* unit test: read fasta file and print all entries */
int main(int argc, char * argv[])
{
  int i;
  multifasta *mf;

  if (argc < 2)
  {
    fprintf (stderr,"ERROR: no FASTA-file specified!");
    return 1;
  }
  mf = read_fasta_file (argv[1]);

  if (mf == NULL)
    return 1;
  
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
