/* representation of multifasta files */

#ifndef FASTA
#define FASTA

/* representation of one fasta entry */
typedef struct fastaStruct {
  char * header; /* header string */
  char * sequence; /* sequence string */
  int length; /* length of sequence */
  int clength; /* length of header */
  int ssize; /* reserved size for sequence (internal) */
  int csize; /* reserved size for header (internal) */

} fasta;

/* representation of a multifasta file */
typedef struct multifastaStruct {
  int length; /* number of entries */
  int maxlen; /* reserved size of entry-list (internal) */
  fasta** entries; /* array of entries */
} multifasta;

/* free memory of multifasta */
void multifasta_delete (multifasta*);
/* read multifasta from file 
   returns NULL if error */
multifasta* read_fasta_file (char*);

#endif
