#ifndef ALIGNMENT
#define ALIGNMENT

#define INF 1000000

// Definition der Struktur fuer eine Multi-Edit-Operation
typedef struct multieditStruct {
  int stretch;
  char operation;
} multiedit;

// Definition der Struktur fuer ein Alignment
typedef struct alignmentStruct {
  char* s1;
  int len1;
  char* s2;
  int len2;

  multiedit** edits;
  int editlength;
  int length;
  int maxlen;

} alignment;

// foward Deklarationen der Funktionen auf einem Alignment
multiedit* multiedit_new (char);
alignment* alignment_new (char*, int, char*, int);
int alignment_add_operation (alignment*, char);
int alignment_add_operations (alignment*, char, int);
int alignment_add_deletion (alignment*);
int alignment_add_deletions (alignment*, int);
int alignment_add_insertion (alignment*);
int alignment_add_insertions (alignment*, int);
int alignment_add_replacement (alignment*);
int alignment_add_replacements (alignment*, int);
int alignment_evalcost (alignment*, int(*)(char,char));
int alignment_show (alignment*);
void alignment_delete (alignment*);
#endif
