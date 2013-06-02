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

static void (*word_visit)(char *);
static void v(void *var) { word_visit((char *) var); }

static void print_word2(void *word)
{
    int s = 0; char *w = (char *) word;
    while(w[s] != ' ') putchar(w[s++]); putchar(' ');
}

static void print_lemma_word(void *lemma)
{ 
    int s; Lemma l = (Lemma) lemma;
    for(s = 0; l->lemma[s] != ']'; s++) putchar(l->lemma[s]);
    printf(": "); list_select(l->words, print_word2); putchar('\n');
}

static void print_lemmas(void *lemma)
{
    int s; Lemma l = (Lemma) lemma;
    for(s = 0; l->lemma[s] != ']'; s++) putchar(l->lemma[s]);
    printf("\n");
}

static int cmp(void *a, void *b)
{ 
    char *A = (char *) a, *B = (char *) b; int s;
    for(s = 0; A[s] != ' ' && B[s] != ' '; s++);
    return strncmp(A, B, s * sizeof(char)); 
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
    else if(!list_search(query->words, word, cmp))
        list_insert(query->words, word);
}

void lemma_print_words(Lemma lemma)
    { list_select(lemma->words, print_word); }

void lemma_list_words(char *lemma, void(*visit)(char *))
{
    Lemma l = (Lemma) STsearch(lemmas, lemma);
    word_visit = visit; if(l == NULL) return; 
    list_select(l->words, v);
}

void lemma_print_lemma_word() { STsort(lemmas, print_lemma_word); }
void lemma_print_lemmas() { STsort(lemmas, print_lemmas); }

int lemma_total_lemmas() { return STcount(lemmas); }
