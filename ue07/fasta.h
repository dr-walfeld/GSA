#ifndef FASTA
#define FASTA

typedef struct fastaStruct {
  char * header;
  char * sequence;
  int length;
  int clength;
  int ssize;
  int csize;

} fasta;

typedef struct multifastaStruct {
  int length;
  int maxlen;
  fasta** entries;
} multifasta;

fasta* fasta_new (int);
void fasta_delte (fasta*);
void fasta_add_base (fasta*, char);
void fasta_add_header_char (fasta*, char);
multifasta* multifasta_new (int);
void multifasta_delete (multifasta*);
void multifasta_add_fasta (multifasta*, fasta*);
multifasta* read_fasta_file (char*);

#endif
