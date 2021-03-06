#ifndef SCOREMATRIX
#define SCOREMATRIX

typedef struct scorematrixStruct {
  int** matrix;
  int indel;
} scorematrix;

scorematrix* initialize_scorematrix (int,int);
void delete_scorematrix(scorematrix*);
int get_score (scorematrix*, char, char);
scorematrix* read_scorematrix (char*,int);
void show_scorematrix(scorematrix*);

#endif
