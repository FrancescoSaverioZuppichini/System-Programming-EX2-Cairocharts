//
//  my_string.h
//  String_Implementation
//
//  Created by Francesco  on 03/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#ifndef my_string_h
#define my_string_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN_CAP 2

#define get_my_string(my_string)(my_string->string)

typedef struct my_string_struct{
    size_t cap;
    size_t size;
    size_t inner_size;
    char *string;
}my_string;

my_string * my_string_init();
char *  my_string_add(my_string *,char);
char *  my_string_add_str(my_string *,char*);

char * my_string_copy_str(my_string * , char * );

void  my_string_destroy(my_string *);
void  my_string_erase(my_string *);

size_t   my_string_get_size(my_string *);
size_t   my_string_get_cap(my_string *);

#endif /* my_string_h */
