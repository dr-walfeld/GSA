#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

// hash function two
// generates odd number < m
inline int h2(int k, int n)
{
  return (1+2*(k%n));
}

// hash function one (m has to be power of two)
inline int h1(int k, int m)
{
  return k%m;
}

// function for double hashing
inline int h(int k, int i, int m)
{
  return (h1(k,m)+i*h2(k,m/2))%m;
}

void hash_delete (hash* ha, int key)
{
  int i = 0;
  int m = ha->m;
  int j;
  do
  {
    j = h(key,i,m);
    if (ha->hashtable[j][0] == key)
      ha->hashtable[j][0] = -2;
    else
      i++;
  } while (ha->hashtable[j][0] != -1 && i < m);
}

inline int hash_search(hash* ha, int key)
{
  int i = 0;
  int j;
  int m = ha->m;
  do
  {
    j = h(key,i,m);
    if (ha->hashtable[j][0] == key)
      return ha->hashtable[j][1];
    else
      i++;
  } while (ha->hashtable[j][0] != -1 && i < m);

  return -1;
}

int hash_insert (hash* ha, int key, int value)
{
  int i = 0;
  int j;
  int m = ha->m;
  do
  {
    j = h(key,i,m);
    if (ha->hashtable[j][0] == -1 || ha->hashtable[j][0] == -2)
    {
      ha->hashtable[j][0] = key;
      ha->hashtable[j][1] = value;
      return j;
    }
    else
      i++;
  } while (i < m);

  return -1;
}


hash* hashtable_new (int m)
{
  hash* temp = (hash*) malloc(sizeof(hash));
  int len = 2;
  while (len <= m)
  {
    len *= 2;
  }
  temp->m = len;
  temp->hashtable = (int**) malloc(len*sizeof(int*));
  int i;
  for (i = 0; i < len; i++)
  {
    temp->hashtable[i] = (int*) calloc(2, sizeof(int));
    temp->hashtable[i][0] = -1;
  }
  temp->h = h;

  return temp;
}

void hashtable_delete (hash* ha)
{
  if (ha == NULL)
    return;

  free(ha->hashtable);
  free(ha);
}

#ifdef UNIT_TEST
int main()
{
  int c;
  int j,val;
  hash* ha = hashtable_new(100);
  for (c = '-'; c <= 'z'; c++)
  {
    j = hash_insert(ha, c, c-'a');
    printf("%c(%d) : %d\n", c, c-'a', j);
  }

  printf("-----------\n");
  for (c = '-'; c <= 'z'; c++)
  {
    val = hash_search(ha,c);
    printf("%c : %d\n", c, val);
  }

  hashtable_delete(ha);
  return 0;
}
#endif
