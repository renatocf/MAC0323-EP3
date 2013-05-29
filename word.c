/* Bibliotecas-padrão */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Bibliotecas internas */
#include "ST.h"
#include "list.h"

#define SEPARATOR ' '

ST words;

typedef struct word *Word;
struct word
{
    char *lemma;
    List sentences;
    List annotated;
};

static void *key(void *word)
    { return word; }

static int eq(void *word1, void *word2)
    { return strcmp((char *) word1, (char *) word2) == 0; }

static int less(void *word1, void *word2)
    { return strcmp((char *) word1, (char *) word2) < 0; }

void word_table_init()
{
    words = STinit(NULL, key, eq, less);
}

void word_table_insert(char *word)
{
    char *aux; int i = 0;
    for(aux = word; *aux != SEPARATOR; aux++, i++);
}
