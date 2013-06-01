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
#include "Item.h"
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

struct word
{
    char *word;
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

static Key key(Item w)
    { Word aux = (Word) w; return (Key) aux->word; }

static int eq(Key word1, Key word2)
{
    char *w1 = (char *) word1; char *w2 = (char *) word2; int s;
    for(s = 0; w1[s] != ' ' && w2[s] != ' '; s++);
    return strncmp(w1, w2, s * sizeof(char)) == 0; 
}

static int less(Key word1, Key word2)
{ 
    char *w1 = (char *) word1; char *w2 = (char *) word2; int s;
    for(s = 0; w1[s] != ' ' && w2[s] != ' '; s++);
    return strncmp(w1, w2, s * sizeof(char)) < 0; 
}

static void word_free(void *word)
{
    Word w = (Word) word; 
    list_free(w->identifiers);
    list_free(w->sentences);
    list_free(w->annotated);
    free(w);
}

static void print_identifier(void *phrase)
{
    char *init = (char *) phrase; int s = 0;
    printf("-------------------------------------------------------\n");
    for(s = 0; init[s] != '\n'; s++) putchar(init[s]);
}

static void print_sentence(void *phrase)
{
    char *init = (char *) phrase; int s = 0; size_t size = 6 * sizeof(char);
    printf("-------------------------------------------------------\n");
    while(init[s] != '[' && strncmp(&init[s], "[Text=", size) != 0) putchar(init[s++]);
}

static void print_annotated(void *phrase)
{
    char *init = (char *) phrase; int s = 0;
    printf("-------------------------------------------------------\n");
    for(s = 0; init[s] != '\n'; s++) putchar(init[s]);
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                  FUNÇÕES DE MANIPULAÇÃO DA TABELA
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void word_table_init()
    { words = STinit(NULLitem, word_free, key, eq, less); }

void word_table_free()
    { STfree(words); }

Word word_table_get(char *word)
    { return (Word) STsearch(words, word); }

void word_table_insert(char *word, char *lemma, 
    char *identifier, char *sentence, char *annotated)
{
    Word query;
    if((query = STsearch(words, word)) == NULLitem)
    {
        /* Cria nova palavra a ser inserida na tabela */
        query = (Word) malloc(sizeof(*query));
        query->word = word; query->lemma = lemma;
        
        /* Insere identificador */
        query->identifiers = list_init();
        list_insert(query->identifiers, (void *) identifier);
        
        /* Insere sentença */
        query->sentences = list_init();
        list_insert(query->sentences, (void *) sentence);
        
        /* Insere sentença anotada */
        query->annotated = list_init();
        list_insert(query->annotated, (void *) annotated);
        
        STinsert(words, query);
    }
    else 
    {
        int current_n, last_n;
        char *last_id = list_item(list_last(query->identifiers));
        sscanf(identifier, "Sentence #%d", &current_n);
        sscanf(last_id, "Sentence #%d", &last_n);
        
        if(current_n != last_n)
        {
            list_insert(query->identifiers, identifier);
            list_insert(query->sentences, sentence);
            list_insert(query->annotated, annotated);
        }
    }
}

void word_print_sentences(Word word)
    { if(word != NULL) list_select(word->sentences, print_sentence); }

void word_print_annotateds(Word word)
    { if(word != NULL) list_select(word->annotated, print_annotated); }

void word_print_identifiers(Word word)
    { if(word != NULL) list_select(word->identifiers, print_identifier); }

char *word_lemma(Word word)
    { return word->lemma; }
