/* encode q-words as integer */
#ifndef TRANSLATE
#define TRANSLATE

/* translate single character (character, alphabet, fail) 
   sets fail-flag if error */
inline int translate(int, int*, int*);
/* precalculate powers of (r and q) */
int* get_pot_list(int, int);
/* translate complete sequence into array of integers
   (sequence, length, q, alphabet-length, alphabet);
  returns NULL if error during encoding */
int* translate_sequence(char*, int, int, int, int*);
/* translate qword into integer code
   (sequence, q, alphabet, powers 0..r^q-1) 
   return -1 if error */
inline int translate_qword(char*, int, int*, int*);
/* create new alphabet-encoding from string 
   return NULL if error */
int* new_alphabet(char*);
#endif
