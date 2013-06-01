#ifndef H_ST_DEFINED
#define H_ST_DEFINED

/* Bibliotecas internas */
#include "Item.h"

/* Tipo Tabela de Símbolos */
typedef struct st *ST;

ST STinit        (Item NULLitem, 
                  void (*free_item) (Item),
                  Key  (*key) (Item),
                  int  (*eq)  (Key, Key),
                  int  (*less)(Key, Key)
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
