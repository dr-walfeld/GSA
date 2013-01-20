#ifndef STACK
#define STACK

typedef struct Stackelement {
  struct Stackelement *next;
  void* data;
} Stackelement;

typedef struct {
  Stackelement *top;
  unsigned size;
} Stack;

Stack* stack_new();
void stack_delete(Stack*);
void stack_push(Stack*, void*);
void* stack_pop(Stack*);
int stack_is_empty(Stack*);
#endif
