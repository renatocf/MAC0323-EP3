/*
 * This code is from "Algorithms in C, Third Edition,"
 * by Robert Sedgewick, Addison Wesley Longman, 1998.
 */
#include <string.h>
#include <stdio.h>

typedef int Item;
typedef int Key;
#define key(A) (A)
#define eq(A, B) ((A) == (B))
#define less(A, B) ((A)<(B))
#define NULLitem (-1)

 int ITEMscan(Item *);
void ITEMshow(Item);
 int ITEMrand();
