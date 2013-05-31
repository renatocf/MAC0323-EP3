/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                               BIBLIOTECAS 
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Bibliotecas-padrão */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Bibliotecas internas */
#include "ST.h"
#include "list.h"
#include "word.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                           MACROS/ESTRUTURAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

#define SEPARATOR ' '
#define NULLitem  NULL

typedef struct word *Word;
struct word
{
    char *lemma;
    List identifiers;
    List sentences;
    List annotated;
};

ST words;

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                            FUNÇÕES INTERNAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

static void *key(void *word)
    { return word; }

static int eq(void *word1, void *word2)
    { return strcmp((char *) word1, (char *) word2) == 0; }

static int less(void *word1, void *word2)
    { return strcmp((char *) word1, (char *) word2) < 0; }


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                            FUNÇÕES EXTERNAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void word_table_init()
{
    words = STinit(NULLitem, key, eq, less);
}

void word_table_insert(char *word, char *lemma, 
    char *identifier, char *sentence, char *annotated)
{
    Word query;
    if((query = STsearch(words, key(word))) == NULLitem)
    {
        /* Cria nova palavra a ser inserida na tabela */
        query = (Word) malloc(sizeof(*query));
        query->lemma = lemma;
        
        /* Insere identificador */
        query->identifiers = list_init();
        list_insert(query->identifiers, identifier);
        
        /* Insere sentença */
        query->sentences = list_init();
        list_insert(query->sentences, sentence);
        
        /* Insere sentença anotada */
        query->annotated = list_init();
        list_insert(query->annotated, annotated);
        
        STinsert(words, query);
    }
    else 
    {
        list_insert(query->identifiers, identifier);
        list_insert(query->sentences, sentence);
        list_insert(query->annotated, annotated);
    }
}
/*
void word_table_free()
{
    
}*/
