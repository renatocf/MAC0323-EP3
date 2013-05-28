#ifndef H_ITEM_DEFINED
#define H_ITEM_DEFINED

#include <stdio.h>
#include <string.h>

typedef void *Item;
typedef char *Key;

#define key(A) (A)
#define eq(A, B) ((strcmp((A) == (B))) == 0)
#define less(A, B) ((strcmp((A)<(B))) < 0)
#define NULLitem NULL

#endif
