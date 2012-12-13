/*
   Repraesentation von Alignments durch eine Folge von Multiedit-Operationen
   fuer Test: Compilieren mit UNIT_TEST Direktive;
   Test-Aufruf mit zwei Sequenzen und folge von Multiedit-Operationen
   z.B. AGTGA AGTGA 2 R 3 I   
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"

// erzeugt neues Multiedit und gibt es zurueck;
// als Parameter wird die Operation erwartet
multiedit* multiedit_new (char operation)
{
  multiedit* temp = (multiedit*)malloc(sizeof(multiedit));
  temp->stretch = 1;
  temp->operation = operation;

  return temp;
}

// erzeugt neues (leeres) Alignment und gibt es zurueck;
// als Parameter werden die beiden Sequenzen (C-Strings) und deren Laenge erwartet
alignment* alignment_new (char* s1, int len1, char* s2, int len2)
{
  alignment* temp = (alignment*)malloc(sizeof(alignment));;
  temp->s1 = s1;
  temp->len1 = len1;
  temp->s2 = s2;
  temp->len2 = len2;

  // hier kann noch optimiert werden, dies ist der worst case
  // edit-length speichert die Anzahl der Edit-Operations-Eintraege
  temp->editlength = 0;
  // length speichert die Anzahl Edit-Operationen = Laenge des Alignments
  temp->length = 0;
  // Reservierung des Speichers fuer die Editoperationen (initialisiert mit 0)
  // im worst-case die maximale Anzahl Editoperationen (m+n)
  temp->edits = (multiedit **) calloc (len1+len2, sizeof (multiedit*));
  // die maximale Laenge wird fuer overflow gespeichert
  temp->maxlen = len1+len2;

  return temp;
}

// vllt. noch Konsistenzpruefung ergaenzen, zur Zeit duerfen
// Alignments gebildet werden, die nicht existieren
// (z.B. nur aus gaps etc.)
int alignment_add_operation (alignment* a, char operation)
{
  if (a->editlength > 0 && a->edits[a->editlength-1]->operation == operation)
  {
    a->edits[a->editlength-1]->stretch++;
  }
  else if (a->editlength == 0 || a->edits[a->editlength-1]->operation != operation)
  {
    // falls doch mal jemand mehr elemente als moeglich hinzufuegen will
    // => ueberlauf verhindern
    if (a->editlength == a->maxlen)
      return 1;
    a->edits[a->editlength] = multiedit_new (operation);
    a->editlength++;
  }
  a->length++; // das gesamte Alignment wird mit jeder Operation um 1 laenger
  return 0;
}

int alignment_add_operations (alignment *a, char operation, int length)
{
  int i;
  int s = 0;
  for (i = 0; i < length && s == 0; i++)
  {
    s += alignment_add_operation (a, operation);
  }

  return s;
}

int alignment_add_deletion (alignment* a)
{
  return alignment_add_operation (a, 'D');
}
int alignment_add_deletions (alignment* a, int length)
{
  return alignment_add_operations (a, 'D', length);
}
int alignment_add_insertion (alignment* a)
{
  return alignment_add_operation (a, 'I');
}
int alignment_add_insertions (alignment* a, int length)
{
  return alignment_add_operations (a, 'I', length);
}
int alignment_add_replacement (alignment* a)
{
  return alignment_add_operation (a, 'R');
}
int alignment_add_replacements (alignment* a, int length)
{
  return alignment_add_operations (a, 'R', length);
}

// reverse the order of the alignment
void alignment_reverse(alignment* a)
{
  int half = a->editlength/2-1;
  int i;
  multiedit* temp;
  for (i = 0; i <= half; i++)
  {
    temp = a->edits[i];
    a->edits[i] = a->edits[a->editlength-1-i];
    a->edits[a->editlength-1-i] = temp;
  }
}

// evaluate the alignment costs
// takes pointer to alignment and function pointer to cost-function
int alignment_evalcost (alignment* a, int (*costFunc) (char, char))
{
  int costs = 0;
  int i,j;
  
  // use two indices (one for each sequence)
  int index1 = 0;
  int index2 = 0;

  multiedit* me = NULL;

  int gap = costFunc ('-', '-'); // gap costs
  // iterate over all entries in multiedit list
  for (i = 0; i < a->editlength; i++)
  {
    me = a->edits[i];
    // iterate over all affected bases from current multiedt operation
    for (j = 0; j < me->stretch; j++)
    {
      // replacement operation
      if (me->operation == 'R')
      {
        // check if current bases in both sequences exist
        // (and if string is not terminated);
        // better not to rely on sequence length
        if (a->s1[index1] == '\0' || a->s2[index2] == '\0')
          return -1;
        // determine costs of current base alignment
        costs += costFunc(a->s1[index1], a->s2[index2]);
        index1++;
        index2++;
      }
      // insert operation
      else if (me->operation == 'I')
      {
        if (a->s2[index2] == '\0')
          return -1;
        costs += gap;
        index2++;
      }
      else
      {
        if (a->s1[index1] == '\0')
          return -1;
        costs += gap;
        index1++;
      }
    }
  }

  return costs;
}

// print out the alignment
int alignment_show (alignment* a)
{
  int i,j;

  // use one index for each sequence ...
  int index1 = 0;
  int index2 = 0;
  // ... and one index for printing
  int printindex = 0;

  // reseve space for C-strings to print
  char * print_s1 = (char*) malloc (a->length+1); // +1 for \0
  char * print_s2 = (char*) malloc (a->length+1); // +1 for \0
  char * print_bt = (char*) malloc (a->length+1); // +1 for \0

  multiedit* me = NULL;

  // iterate over all entries in multiedit-list
  for (i = 0; i < a->editlength; i++)
  {
    me = a->edits[i];
    // apply all single edit-operations
    for (j = 0; j < me->stretch; j++)
    {
      //printf("%c\n", me->operation);
      // check nature of edit operation
      if (me->operation == 'R')
      {
        if (a->s1[index1] == '\0' || a->s2[index2] == '\0')
        {
          printf ("index1=%d, index2=%d\n",index1,index2);
          return 1;
        }
        // chekc euqality of bases for replacement operation
        if (a->s1[index1] == a->s2[index2])
        {
          print_bt[printindex] = '|';
        }
        else
        {
          print_bt[printindex] = ' ';
        }
        print_s1[printindex] = a->s1[index1];
        print_s2[printindex] = a->s2[index2];
        index1++;
        index2++;
      }
      else if (me->operation == 'I')
      {
        if (a->s2[index2] == '\0')
        {
          printf("index2=%d\n",index2);
          return 1;
        }
        print_s1[printindex] = '-';
        print_s2[printindex] = a->s2[index2];
        print_bt[printindex] = ' ';
        index2++;
      }
      else
      {
        if (a->s1[index1] == '\0')
        {
          printf("index1=%d\n",index1);
          return 1;
        }
        print_s1[printindex] = a->s1[index1];
        print_s2[printindex] = '-';
        print_bt[printindex] = ' ';
        index1++;
      }

      printindex++;
    }
  }

  // set zero-termination
  print_s1[printindex] = '\0';
  print_s2[printindex] = '\0';
  print_bt[printindex] = '\0';

  // prints alignment
  printf ("%s\n", print_s1);
  printf ("%s\n", print_bt);
  printf ("%s\n", print_s2);

  // free space of C-strings
  free (print_s1);
  free (print_s2);
  free (print_bt);

  return 0;

}

void alignment_delete (alignment* a)
{
  if (a == NULL)
    return;

  int i;
  for (i = 0; i < a->editlength; i++)
  {
    // free memory of single multioperations
    free(a->edits[i]);
    a->edits[i] = NULL;
  }
  // free memory of array of multioperations
  free (a->edits);
  a->edits = NULL;
  // free memory of alignment itself
  free (a);
  a = NULL;
}

// return 1 if alignments match, 0 otherwise
int alignment_compare (alignment* a, alignment* b)
{
  if (a->editlength != b->editlength)
    return 0;

  int i;
  for (i = 0; i < a->editlength; i++)
  {
    if (a->edits[i]->stretch != b->edits[i]->stretch)
      return 0;
    if (a->edits[i]->operation != b->edits[i]->operation)
      return 0;
  }

  return 1;
}

#ifdef UNIT_TEST
int unit_cost (char, char);

int unit_cost (char a, char b)
{
  if (a == '-' || b == '-')
    return 1;
  else if (a == b)
    return 0;
  return 1;
}

int main(int argc, char * ARGV[])
{
  if (argc < 5 || argc%2 == 0)
  {
    printf("FEHLER: falsche Anzahl Argumente!\n");
    return 1;
  }

  char * s1 = ARGV[1];
  int len1 = strlen(s1);
  char * s2 = ARGV[2];
  int len2 = strlen(s2);

  int ops = 0;

  int i;
  alignment *al = alignment_new (s1, len1, s2, len2);
  for (i = 3; i < argc; i+=2)
  {
    ops |= alignment_add_operations (al, ARGV[i][0], atoi(ARGV[i+1]));
  }

  if (ops)
  {
    printf ("Fehler beim Einfuegen einer Operation (Ueberlauf)!\n");
  }

  int show = alignment_show (al);

  if (show)
  {
    printf ("Fehler beim Ausgeben des Alignments (Alignment inkonsistent)!\n");
  }

  printf ("Länge des Alignments (Länge der Multiedit-Liste): %d (%d)\n", \
      al->length, al->editlength);

  int costs = alignment_evalcost (al, unit_cost);
  printf ("Kosten des Alignments: %d\n", costs);
  alignment_delete (al);
  al = NULL;

  return 0;
}
#endif
