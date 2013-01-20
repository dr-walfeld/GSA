#ifndef PSSM
#define PSSM

typedef struct pssmStruct {
  int** matrix;
  int* maxscore;
  int m,n;
} Pssm;

void delete_pssm (Pssm*);
inline int get_pssmscore (Pssm*, int, char);
Pssm* read_pssm (char*);
inline int get_pssmmaxscore (Pssm*, int);

#endif
