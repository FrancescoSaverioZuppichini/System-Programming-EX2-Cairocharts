//
//  my_string.c
//  String_Implementation
//
//  Created by Francesco  on 03/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#include "my_string.h"
my_string * my_string_init(){
    my_string * my_new_string;
    
    my_new_string = malloc(sizeof(my_string));
    if(!my_new_string)
        return NULL;
    my_new_string->cap = MIN_CAP;
    my_new_string->size = 0;
    my_new_string->inner_size = 0;

    my_new_string->string = malloc(sizeof(my_new_string->cap));
    if(!my_new_string->string)
        return NULL;
    return my_new_string;
}

char *  my_string_add(my_string * this,const char el){

    if(this->inner_size == this->cap -1){
        this->cap *= 2;
        this->string = realloc(this->string,this->cap);
        if(!this->string)
            return NULL;
    }
    this->string[this->size] = el;
    if(this->size == this->inner_size )
        this->inner_size++;
    if(el != '\0'){
        this->size++;
        this->string[this->size] = '\0';
    }
    return this->string;
}

char *  my_string_add_str(my_string *this,const char* src){
    int i;
    if(this->inner_size == 0)
        return NULL;
    for(i = 0; i <strlen(src) + 1;i++){
        my_string_add(this, src[i]);
    }
    
    return this->string;
}


char * my_string_copy_str(my_string * this,const char * src){
    size_t new_size;
    int i;
    unsigned long len;
    
    if(src == NULL)
        return NULL;
    
    len = strlen(src);
    
    if(len == 0)
        return NULL;
        
    new_size = strlen(src) + 1;
    
    my_string_erase(this);
    
    for(i = 0; i < new_size; i++){
        my_string_add(this, src[i]);
    }
   
    return this->string;

}

void my_string_erase(my_string *this){
    this->size = 0;
}

void  my_string_destroy(my_string * this){
    free(this->string);
    free(this);
}

size_t   my_string_get_size(my_string * this){
    return this->size;
}

size_t   my_string_get_cap(my_string * this){
    return this->cap;
}