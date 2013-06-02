#ifndef H_WORD_DEFINED
#define H_WORD_DEFINED

/* Definição de palavra */
typedef struct word *Word;

/* Funções para manipulação da tabela */
Word  word_table_get    (char *word);
void  word_table_init   (void);
void  word_table_free   (void);
void  word_table_insert (char *word, 
                         char *lemma, 
                         char *identif, 
                         char *sentence, 
                         char *annotated);

/* Funções para manipulação da palavra */
char *word_lemma               (Word);
int   word_total_words         (void);
void  word_print_words         (void);
int   word_total_tokens        (void);
void  word_print_tokens        (void);
void  word_print_sentences     (Word);
void  word_print_annotateds    (Word);
void  word_print_identifiers   (Word);

#endif
