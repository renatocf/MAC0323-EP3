/***********************************************/
/**  MAC 0323  -  Estrutura de Dados          **/
/**  IME-USP   -  Primeiro  Semestre de 2013  **/
/**  Turma 45  -  Yoshiharu Kohayakawa        **/
/**                                           **/
/**  Primeiro  Exercício-Programa             **/
/**  Arquivo:  list-internal.h                **/
/**                                           **/
/**  Renato Cordeiro Ferreira        7990933  **/
/***********************************************/ 

#ifndef H_LIST_INTERNAL
#define H_LIST_INTERNAL

#include "list.h"

struct lnode {
    L_Item item;
    Link next;
};

struct list {
    Link head;
    Link last;
};

#endif
