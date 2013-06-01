#ifndef H_LEMMA_DEFINED
#define H_LEMMA_DEFINED

/* Definição de palavra */
typedef struct lemma *Lemma;

/* Funções para manipulação da tabela */
Lemma lemma_table_get    (char *lemma);
void  lemma_table_init   (void);
void  lemma_table_free   (void);
void  lemma_table_insert (char *lemma, char *word);

/* Funções para manipulação da palavra */
void  lemma_print_words  (Lemma);
void  lemma_list_words   (char *lemma, void(*visit)(char *));

#endif
