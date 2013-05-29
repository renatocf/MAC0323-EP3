/***********************************************/
/**  MAC 0323  -  Estrutura de Dados          **/
/**  IME-USP   -  Primeiro  Semestre de 2013  **/
/**  Turma 45  -  Yoshiharu Kohayakawa        **/
/**                                           **/
/**  Primeiro  Exercício-Programa             **/
/**  Arquivo:  list.c                         **/
/**                                           **/
/**  Renato Cordeiro Ferreira        7990933  **/
/***********************************************/ 

#include<stdlib.h>
#include "list-internal.h"

List list_init()
{
    List new = (List) malloc(sizeof(*new));
    Link head = (Link) malloc(sizeof(*head));
    
    new->head = head;
    new->last = head;
    new->last->next = NULL;
    
    return new;
}

void list_insert(List list, L_Item item)
{
    Link new = (Link) malloc(sizeof(*new));
    Link last = list->last;
    
    last->next = new;
    new->next = NULL;
    list->last = new;
    
    new->item = item;
}

L_Item list_remove(List list, Link node)
{
    L_Item dead; Link aux;

    for(aux = list->head; aux->next != node; aux = aux->next);
    
    if(node == list->last) list->last = aux;
    aux->next = node->next;
    
    dead = node->item;
    free(node);
    
    return dead;
}

int list_empty(List list)
{
    return list->head == list->last;
}

void list_free(List list)
{
    while(!list_empty(list))
        list_remove(list, list->last);
    
    free(list->head); free(list);
}

Link list_first(List list)
{
    return list->head->next;
}

Link list_next(Link node)
{
    if(node == NULL) return NULL;
    return node->next;
}

L_Item list_item(Link node)
{
    L_Item send;
    send = node->item;
    return send;
}

L_Item *list_item_a(Link node)
{
    L_Item *send;
    send = &node->item;
    return send;
}
