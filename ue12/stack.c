#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "stack.h"

Stackelement* stackelement_new()
{
  Stackelement *se;
  se = malloc(sizeof(Stackelement));
  assert(se);
  /* initialize next element with NULL */
  se->next = NULL;
  return se;
}

void stackelement_delete(Stackelement* se)
{
  /* free memory and set pointer to NULL*/
  if (se != NULL)
  {
    free(se);
    se = NULL;
  }
}

Stack* stack_new()
{
  Stack *s;
  s = malloc(sizeof(Stack));
  assert(s);
  /* initialize first element with NULL */
  s->top = NULL;
  return s;
}

void stack_delete(Stack *s)
{
  /* delete all elements starting from top */
  if (s != NULL)
  {
    Stackelement* se = s->top;
    Stackelement* next;
    while (se != NULL)
    {
      next = se->next;
      stackelement_delete(se);
      se = next;
    }
    free(s);
    s = NULL;
  }
}

void stack_push(Stack *s, void* data)
{
  assert(s);
  /* create new stackelement, write data and set element to top of stack */
  Stackelement* se = stackelement_new();
  se->data = data;
  se->next = s->top;
  s->top = se;
}

void* stack_pop(Stack *s)
{
  void* retval;
  assert(s);
  /* assert stack is not empty */
  assert(!stack_is_empty(s));
  /* store data-value, set top to next element and delete old top element */
  Stackelement* se = s->top;
  retval = se->data;
  s->top = se->next;
  stackelement_delete (se);

  return retval;
}

int stack_is_empty(Stack *s)
{
  assert(s);
  /* stack is empty if first element is NULL */
  return (s->top == NULL);
}
