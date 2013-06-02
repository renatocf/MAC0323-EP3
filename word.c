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

#define SEPARATOR     ' '
#define NULLitem      NULL
#define BUF_CMP_SIZE  500
#define PRT_SEPARATOR '\n'

struct sentence
{
    List identifiers;
    List sentences;
    List annotated;
}

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
    char *w1 = (char *) word1, *w2 = (char *) word2; 
    char W1[BUF_CMP_SIZE], W2[BUF_CMP_SIZE]; int s = 0;
    for(s = 0; w1[s] != ' '; s++) W1[s] = w1[s]; W1[s] = '\0';
    for(s = 0; w2[s] != ' '; s++) W2[s] = w2[s]; W2[s] = '\0';
    return strcmp(W1, W2) == 0;
}

static int less(Key word1, Key word2)
{ 
    char *w1 = (char *) word1, *w2 = (char *) word2; 
    char W1[BUF_CMP_SIZE], W2[BUF_CMP_SIZE]; int s = 0;
    for(s = 0; w1[s] != ' '; s++) W1[s] = w1[s]; W1[s] = '\0';
    for(s = 0; w2[s] != ' '; s++) W2[s] = w2[s]; W2[s] = '\0';
    return strcmp(W1, W2) < 0;
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
    for(s = 0; init[s] != '\n'; s++) putchar(init[s]);
}

static void print_sentence(void *phrase)
{
    char *init = (char *) phrase; int s = 0; size_t size = 6 * sizeof(char);
    while(init[s] != '[' && strncmp(&init[s], "[Text=", size) != 0) putchar(init[s++]);
}

static void print_annotated(void *phrase)
{
    char *init = (char *) phrase; int s = 0;
    for(s = 0; init[s] != '\n'; s++) putchar(init[s]);
    putchar('\n');
}

static void print_tokens(void *word)
{
    int s; Word w = (Word) word;
    for(s = 0; w->word[s] != ' '; s++) putchar(w->word[s]); 
    putchar(PRT_SEPARATOR);
}

static void print_words(void *word)
{
    int s; Word w = (Word) word;
    for(s = 0; w->word[s] != ' '; s++)
        if(w->word[s] < 'A' || (w->word[s] > 'Z' && w->word[s] < 'a')
        || w->word[s] > 'z') return;
    printf("%.*s", s, w->word); putchar(PRT_SEPARATOR);
}

static int n_words = 0;
static void count_words(void *word)
{
    int s; Word w = (Word) word;
    for(s = 0; w->word[s] != ' '; s++)
        if(w->word[s] < 'A' || (w->word[s] > 'Z' && w->word[s] < 'a')
        || w->word[s] > 'z') return;
    n_words++;
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

void word_print_identifiers(Word word)
    { if(word != NULL) list_select(word->identifiers, print_identifier); }

void word_print_annotateds(Word word)
    { if(word != NULL) list_select(word->annotated, print_annotated); }

void word_print_sentences(Word word, int verbosity)
{ 
    if(word != NULL) 
    {
        printf("\n-------------------------------------------------------\n");
        if(verbosity) word_print_identifiers(word);
        list_select(word->sentences, print_sentence); 
        if(verbosity > 1) word_print_annotateds(word);
    }
}

char *word_lemma(Word word)
    { if(word != NULL) return word->lemma; else return NULL; }

void word_print_words() { STsort(words, print_words); }
void word_print_tokens() { STsort(words, print_tokens); }

int word_total_words()  { STsort(words, count_words); return n_words; }
int word_total_tokens() { return STcount(words); }
