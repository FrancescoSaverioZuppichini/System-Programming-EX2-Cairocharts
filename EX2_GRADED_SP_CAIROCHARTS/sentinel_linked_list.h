//
//  sentinel_linked_list_int.h
//  SentinelLinkedList
//
//  Created by Francesco  on 07/04/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#ifndef sentinel_linked_list_h
#define sentinel_linked_list_h
#define EXPLICIT_CAST(pointer,type)((type *)(pointer))
/* This macro needs the current node and the type of your data.
 It returns the pointer to the casted data */
#define sll_get_data(node,type)((type*)(EXPLICIT_CAST(node,sll_node))->data)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sentinel_linked_list sll;
typedef struct sentinel_linked_list_node sll_node;

struct sentinel_linked_list_node{

    void *data;
    struct sentinel_linked_list_node * next;
    struct sentinel_linked_list_node * prev;

};

struct sentinel_linked_list{

    long size;
    int data_size;
    struct sentinel_linked_list_node * head;
    struct sentinel_linked_list_node * tail;
};

/* Return a new sentinel_linked_list */
extern sll *sll_init(int);
/* Destroy the given sentinel_linked_list */
extern void sll_destroy(const sll *);
/* Append a element to given sentinel_linked_list */
extern sll_node * sll_append(sll *, void * el);
/* Return the node with the given element */
extern sll_node *sll_get(const sll *, void * );
/* Remove and return the node with the given element */
extern sll_node* sll_remove(sll *, void * );
/* Remove and return the last node in the list*/
extern sll_node * sll_pop_back(sll *);
/* Remove and return the last first in the list*/
extern sll_node * sll_pop_front(sll *);
/* Map a function to every node in the list */
extern void sll_map(const sll *, void (*)(void *));
/* Print all the list */
extern void sll_print(const sll *,void (*)(void*));

#endif /* sentinel_linked_list_h */
