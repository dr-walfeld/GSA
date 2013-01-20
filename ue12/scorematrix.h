/*
   representation of scorematrix
*/
#ifndef SCOREMATRIX
#define SCOREMATRIX

/* scorematrix */
typedef struct scorematrixStruct {
  int** matrix; /* matrix itself */
  int* maxscore; /* array of maximal scores for each character */
  int indel; /* indel score */
  int size; /* size of scorematrix */
} scorematrix;

/* free memory of scorematrix */
void delete_scorematrix(scorematrix*);
/* get score of scorematrix and two characters (* or [ for STOP) 
   return INT_MIN if error */
inline int get_score (scorematrix*, char, char);
/* read scorematrix from file (and set specified indel scores */
scorematrix* read_scorematrix (char*,int);
/* get maximum score for character
   returns INT_MIN if error */
inline int get_maxscore (scorematrix*, char);

#endif
