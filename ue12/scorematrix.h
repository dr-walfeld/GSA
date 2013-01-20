#ifndef SCOREMATRIX
#define SCOREMATRIX

typedef struct scorematrixStruct {
  int** matrix;
  int* maxscore;
  int indel;
  int size;
} scorematrix;

scorematrix* initialize_scorematrix (int,int);
void delete_scorematrix(scorematrix*);
inline int get_score (scorematrix*, char, char);
scorematrix* read_scorematrix (char*,int);
void show_scorematrix(scorematrix*);
inline int get_maxscore (scorematrix*, char);

#endif
