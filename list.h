/***********************************************/
/**  MAC 0323  -  Estrutura de Dados          **/
/**  IME-USP   -  Primeiro  Semestre de 2013  **/
/**  Turma 45  -  Yoshiharu Kohayakawa        **/
/**                                           **/
/**  Primeiro  Exercício-Programa             **/
/**  Arquivo:  list.h                         **/
/**                                           **/
/**  Renato Cordeiro Ferreira        7990933  **/
/***********************************************/ 

#ifndef H_LIST_DEFINED
#define H_LIST_DEFINED

/* Bibliotecas */
#include "list-item.h"

/* Lista e link para nódulo de lista */
typedef struct lnode *Link;
typedef struct list *List;

/* Funções para uso do cliente */
List    list_init   (void);
L_Item  list_item   (Link);
void    list_free   (List);
Link    list_next   (Link);
int     list_empty  (List);
Link    list_first  (List);
L_Item *list_item_a (Link);
void    list_insert (List, L_Item);
L_Item  list_remove (List, Link);

#endif
