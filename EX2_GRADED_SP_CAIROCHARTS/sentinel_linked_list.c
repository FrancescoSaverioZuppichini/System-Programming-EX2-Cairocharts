//
//  sentinel_linked_list_int.c
//  SentinelLinkedList
//
//  Created by Francesco  on 07/04/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#include "sentinel_linked_list.h"

sll * sll_init(int size_of_data){
    
    sll *new_sll = malloc(sizeof(sll));
    sll_node *sentinel_node = malloc(sizeof(sll_node));

    if(!new_sll || !sentinel_node)
        return NULL;
   
    new_sll->data_size = size_of_data;
    new_sll->head = sentinel_node;
    new_sll->tail = sentinel_node;
    sentinel_node->next = sentinel_node;
    sentinel_node->prev = sentinel_node;

    
    return new_sll;
}

sll_node * sll_append(sll * this, void *el){
    sll_node *new_node;
    
    new_node = malloc(sizeof(sll_node));
    
    if(!new_node)
        return NULL;
    /* allocate the memory for data */
    new_node->data = malloc(this->data_size);
    /* copy el in data */
    memcpy(new_node->data, el, this->data_size);
    /* update the pointers references */
    this->tail->next = new_node;
    new_node->prev = this->tail;
    /* append the new node */
    this->tail = new_node;
    this->tail->next = this->head;
    this->head->prev = new_node;
    
    this->size++;
    
    return new_node;
    
}

sll_node * sll_remove(sll * this, void *el){
    sll_node *to_remove;
    /* find the element */
    to_remove = sll_get(this, el);
    
    if(!to_remove)
        return NULL;
    /* update the pointers references */
    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    
    return to_remove;
}

sll_node * sll_pop_front(sll *this){
    sll_node *to_pop;
    
    if(this->size == 0)
        return NULL;
    to_pop = this->head->next;
    /* update pointers */
    this->head->next = to_pop->next;
    to_pop->next->prev = this->head;
    
    this->size--;
    return to_pop;
}

sll_node * sll_pop_back(sll *this){
    sll_node *to_pop;
    
    if(this->size == 0)
        return NULL;
    
    to_pop = this->tail;
    /* update pointers */
    to_pop->next->prev = to_pop->prev;
    to_pop->prev->next = this->head;
    this->tail = to_pop->prev;
    
    this->size--;
    
    return to_pop;
}

void sll_print(const sll * this, void (*custom_print)(void*)){
    
    sll_map(this, custom_print);
}
/* This function performs a action on every node */
void sll_map(const sll * this, void (*action)(void *)){
    sll_node * pos;
    /* list is empty */
    if(this->size == 0)
        return;
    /* call action on every node */
    for(pos = this->head->next; pos != this->head; pos = pos->next){
        action(pos);
    }
}

sll_node *sll_get(const sll * this, void *el){
    sll_node * pos;
    
    for(pos = this->head->next; pos != this->head; pos = pos->next){
        if(pos->data == el)
            break;
    }
    
    if(pos == this->head)
        return NULL;
    
    return pos;
}
/* This function is collect by sll_destroy it's porpuse 
 is to erase all data from a given node */
void inner_destroy(void *node){
    sll_node *curr = (sll_node*)node;
    if(!curr->data)
        return;
    /* byre bye curr */
    free(curr->data);
    free(curr);
}
/* This function free al memory of a given list */
void sll_destroy(const sll * this){
    if(this){
        sll_map(this,&inner_destroy);
    }
    
}