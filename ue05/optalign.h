#ifndef OPTALIGN
#define OPTALING

#define MIN(a,b,c) (a <= b ? (a <= c ? a : c) : (b <= c ? b : c))
#define INF 1000000

typedef struct alignentryStruct {
  int value;
  int north;
  int west;
  int northwest;
} alignentry;

int align (alignentry***, char*, int, char*, int, int (*) (char,char));
int unity (char, char);
int hamming (char, char);
int otherCosts (char, char);
alignentry*** initializeDP (int, int);
void deleteDP (alignentry***, int, int);
alignentry* alignentry_new (int, int, int, int);
int traceback (alignentry***, alignment*, int, int);

#endif
