#ifndef H_ST_DEFINED
#define H_ST_DEFINED

/* Bibliotecas internas */
#include "Item.h"

/* Tipo Tabela de Símbolos */
typedef struct st *ST;

ST STinit        (void *NULLItem, 
                  void  (*free_item) (Item),
                  void *(*key) (void *),
                  int   (*eq)  (void *,void *),
                  int   (*less)(void *,void *)
                 );
void STsort      (ST, void(*visit)(Item));
void STfree      (ST);
int  STcount     (ST);
int  STempty     (ST);
void STdelete    (ST, Key);
Item STsearch    (ST, Key);
void STinsert    (ST, Item);
void STdeleteMin (ST);
void STdeleteMax (ST);

#endif
