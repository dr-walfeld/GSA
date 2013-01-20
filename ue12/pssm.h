/* representation of position specific scoring matrix */

#ifndef PSSM
#define PSSM


/* representation of PSSM */
typedef struct pssmStruct {
  int** matrix; /* PSSM */
  int* maxscore; /* maximal score for each position in PSSM */
  int characters,positions; /* number of characters and number of positions in PSSM */
} Pssm;

/* free memory of PSSM */
void delete_pssm (Pssm*);
/* get score of (pssm, position, character) */
inline int get_pssmscore (Pssm*, int, char);
/* read Pssm from file */
Pssm* read_pssm (char*);
/* get maximal sore in pssm at position */
inline int get_pssmmaxscore (Pssm*, int);

#endif
