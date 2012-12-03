#ifndef OPTALIGN
#define OPTALING

#include "scorematrix.h"

#define MAX(a,b) (a > b ? a : b)

typedef struct alignentryStruct {
  int value;
  int north;
  int west;
  int northwest;
} alignentry;

int align (alignentry***, char*, int, char*, int, scorematrix* sm,int*,int*);
alignentry*** initializeDP (int, int);
void deleteDP (alignentry***, int, int);
alignentry* alignentry_new (int, int, int, int);
int traceback (alignentry***, alignment*, int, int);
void show_DPtable(char*,int,char*,int, alignentry***);

#endif
