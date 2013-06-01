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
#include "lemma.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                           MACROS/ESTRUTURAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

#define SEPARATOR ' '
#define NULLitem  NULL

struct lemma { char *lemma; List words; };
ST lemmas;

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                            FUNÇÕES INTERNAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

static Key key(Item l)
    { Lemma aux = (Lemma) l; return (Key) aux->lemma; }

static int eq(Key lemma1, Key lemma2)
{
    char *l1 = (char *) lemma1; char *l2 = (char *) lemma2; int s;
    for(s = 0; l1[s] != ' ' && l2[s] != ' '; s++);
    return strncmp(l1, l2, s * sizeof(char)) == 0; 
}

static int less(Key lemma1, Key lemma2)
{ 
    char *l1 = (char *) lemma1; char *l2 = (char *) lemma2; int s;
    for(s = 0; l1[s] != ' ' && l2[s] != ' '; s++);
    return strncmp(l1, l2, s * sizeof(char)) < 0; 
}

static void lemma_free(void *lemma)
{
    Lemma l = (Lemma) lemma;
    list_free(l->words);
    free(l);
}

static void print_word(void *phrase)
{
    char *init = (char *) phrase; int s = 0;
    printf("-------------------------------------------------------\n");
    for(s = 0; init[s] != ' '; s++) putchar(init[s]);
}


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                  FUNÇÕES DE MANIPULAÇÃO DA TABELA
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void lemma_table_init()
    { lemmas = STinit(NULLitem, lemma_free, key, eq, less); }

void lemma_table_free()
    { STfree(lemmas); }

Lemma lemma_table_get(char *lemma)
    { return (Lemma) STsearch(lemmas, key(lemma)); }

void lemma_table_insert(char *lemma, char *word)
{
    Lemma query;
    if((query = STsearch(lemmas, lemma)) == NULLitem)
    {
        /* Cria nova palavra a ser inserida na tabela */
        query = (Lemma) malloc(sizeof(*query));
        query->lemma = lemma;
        
        /* Insere palavra cujo lema 
         * é a chave desta estrutura */
        query->words = list_init();
        list_insert(query->words, word);
        
        STinsert(lemmas, query);
    }
    else list_insert(query->words, word);
}

void lemma_print_words(Lemma lemma)
    { list_select(lemma->words, print_word); }
