//
//  main.c
//  EX2_GRADED_SP_CAIROCHARTS
//
//  Created by Francesco  on 01/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#include <stdio.h>
#include "parse_helper.h"
#include <cairo.h>
#include <cairo-pdf.h>
#include "my_string.h"
#include "sentinel_linked_list_int.h"
#include "draw_helper.h"

void free_memory(cairocharts_payload *, sll *);
/* This function takes the data from the stdio and it stores into the custom sentinel linked list */
int get_data_from_std(sll *);
void print_data(cairocharts_payload *, sll * );

void my_print(void *node){
    cairo_point * curr_point;
    
    curr_point = sll_get_data(node,cairo_point);
    printf("(%0.2f, %0.2f) ",curr_point->x,curr_point->y);
}

int main(int argc, const char * argv[]) {
    /* This stuct will holds all the cmmd params */
    cairocharts_payload * my_payload;
    /* This sentinel linking list will holds the float points */
    sll *float_std_sll;
    
    float_std_sll = sll_init(sizeof(cairo_point));
    if(!float_std_sll)
        return EXIT_FAILURE;
    
    /* get data */
    my_payload = get_cairocharts_payload(argc,argv);
    if(!get_data_from_std(float_std_sll))
        return EXIT_FAILURE;
    
    /* print data */
    print_data(my_payload,float_std_sll);
    
    create_cairocharts(my_payload,float_std_sll);
    
    free_memory(my_payload,float_std_sll);
    
    return EXIT_SUCCESS;
}

void print_data(cairocharts_payload * my_payload, sll * my_sll){
    putchar('\n');
    print_payload(my_payload);
    putchar('\n');
    printf("%s","Points: [");
    sll_print(my_sll,&my_print);
    putchar(']');
    putchar('\n');

}

void free_memory(cairocharts_payload * my_payload, sll* my_sll){
    cairocharts_destroy(my_payload);
    sll_destroy(my_sll);
}

int store_float_into_sll(sll *, my_string *);

int get_data_from_std(sll * my_sll){
    my_string * std_string;
    char c;
    
    std_string = my_string_init();
    
    if(!std_string)
        return 0;
    
    for(;(c = getchar());){
        if(c == '\n' || c == EOF){
            break;
        }
        else if(!my_string_add(std_string, c))
                return 0;
    }
    if(!store_float_into_sll(my_sll,std_string))
        return 0;
    return 1;
}

int store_float_into_sll(sll * my_sll, my_string * std_string){
    char *token;
    char *x_y_token;
    char * reserve;
    
    token = strtok(std_string->string, " ");
    while(token){
        cairo_point * curr_point = malloc(sizeof(cairo_point));
        if(strchr(token,',') != NULL){
            x_y_token = strdup(token);
            /* goto Error if the input is not a correct float */
            if(sscanf(strtok_r(x_y_token,",",&reserve),"%f",&curr_point->x) == 0)
               goto parsingError;
            if(sscanf(strtok_r(reserve,",",&reserve),"%f",&curr_point->y) == 0)
                goto parsingError;
        }
        else{
            if(sscanf(token, "%f", &curr_point->y) == 0)
                goto parsingError;
        /* default */
        curr_point->x = my_sll->size;
        }
        sll_append(my_sll, curr_point);
        token = strtok(NULL, " ");
        return 1;
        parsingError:
               puts("Error during parsing");
               return 0;
               
    }
    return 1;
}