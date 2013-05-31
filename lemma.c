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

typedef struct lemma *Lemma;
struct lemma
{
    List words;
};

ST lemmas;

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                            FUNÇÕES INTERNAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

static void *key(void *lemma)
    { return lemma; }

static int eq(void *lemma1, void *lemma2)
    { return strcmp((char *) lemma1, (char *) lemma2) == 0; }

static int less(void *lemma1, void *lemma2)
    { return strcmp((char *) lemma1, (char *) lemma2) < 0; }

static void lemma_free(void *lemma)
{
    Lemma l = (Lemma) lemma;
    list_free(l->words);
    free(l);
}


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                            FUNÇÕES EXTERNAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void lemma_table_init()
{
    lemmas = STinit(NULLitem, lemma_free, key, eq, less);
}

void lemma_table_insert(char *lemma, char *word)
{
    Lemma query;
    if((query = STsearch(lemmas, key(lemma))) == NULLitem)
    {
        /* Cria nova palavra a ser inserida na tabela */
        query = (Lemma) malloc(sizeof(*query));
        
        /* Insere palavra cujo lema 
         * é a chave desta estrutura */
        query->words = list_init();
        list_insert(query->words, word);
        
        STinsert(lemmas, query);
    }
    else list_insert(query->words, word);
}

void lemma_table_free()
{
    STfree(lemmas);
}
