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
#include "sentinel_linked_list.h"
#include "draw_helper.h"

#define ParsinError "Error during parsing the std input"
#define InvalidXError "You can't pass a x coordinate in this graph type"
#define NoPointsError "The graph has no points"


void free_memory(cairocharts_payload *, sll *);
/* This function takes the data from the stdio and it stores into the custom sentinel linked list */
int get_data_from_std(sll *,cairochart_Type);
void smoothing_average(sll ** ,int );
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
    sll *cairo_point_sll;
    
    cairo_point_sll = sll_init(sizeof(cairo_point));
    if(!cairo_point_sll)
        return EXIT_FAILURE;
    
    /* get data */
    my_payload = get_cairocharts_payload(argc,argv);
    if(!my_payload)
        return EXIT_FAILURE;
    if(!get_data_from_std(cairo_point_sll,my_payload->type)){
        puts(ParsinError);
        return EXIT_FAILURE;
    }
    
    if(my_payload->avg_window > 1){
        smoothing_average(&cairo_point_sll,my_payload->avg_window);
    }
#ifdef DEBUG
    /* print data */
    print_data(my_payload,cairo_point_sll);
#endif
    create_cairocharts(my_payload,cairo_point_sll);
    
    free_memory(my_payload,cairo_point_sll);
    
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

int store_float_into_sll(sll *, my_string *,cairochart_Type);

int get_data_from_std(sll * my_sll,cairochart_Type type){
    my_string * std_string;
    char c;
    int error_code;

    error_code = 1;

    std_string = my_string_init();
    
    if(!std_string)
        error_code = 0;
    
    for(;(c = getchar());){
        if(c == '\n' || c == EOF){
            break;
        }
        else if(!my_string_add(std_string, c))
            error_code = 0;
    }
    if(!store_float_into_sll(my_sll,std_string,type))
        error_code = 0;
    if(my_sll->size == 0){
        puts(NoPointsError);
        error_code = 0;
    }
    return error_code;
}

int store_float_into_sll(sll * my_sll, my_string * std_string, cairochart_Type type ){
    char *token;
    /* used to temp store the pair (x,y)*/
    char *x_y_token;
    /* used in strtok_r */
    char * reserve;
    /* bool flag that will tell us if we found a x,y pair in the std */
    int there_is_x;
    /* This will hold the current point read */
    cairo_point * curr_point;
    int error_code;

    error_code = 1;
    curr_point = malloc(sizeof(cairo_point));
    if(!curr_point)
        error_code = 0;
    token = strtok(std_string->string, " ");
    while(token){
        there_is_x = (strchr(token,',') != NULL);
        /* ONLY here x can be allowed */
        if(there_is_x && type == XPLOT){
            x_y_token = strdup(token);
            if(sscanf(strtok_r(x_y_token,",",&reserve),"%f",&curr_point->x) == 0)
                error_code = 0;
            if(sscanf(strtok_r(reserve,",",&reserve),"%f",&curr_point->y) == 0)
                error_code = 0;
        }
        else if(there_is_x){
            /* x should be allowed ONLY in XPLOT*/
            puts(InvalidXError);
            error_code = 0;
        }
        /* up to here there are only a Y in the current token */
        else{
            if(sscanf(token, "%f", &curr_point->y) == 0)
                error_code = 0;
            /* default */
            curr_point->x = my_sll->size;
        }
        sll_append(my_sll, curr_point);
        token = strtok(NULL, " ");
    }
    my_string_destroy(std_string);
    return error_code;

}

void smoothing_average(sll ** my_sll,int avg_windos){
    sll *smoothed_sll;
    sll **temp_sll;
    sll_node *pos;
    sll_node *temp_pos;
    int i,k;
    
    i = 0;
    smoothed_sll = sll_init(sizeof(cairo_point));
    for(pos = (*my_sll)->head->next; pos != (*my_sll)->head && i < (*my_sll)->size - (avg_windos - 1); pos = pos->next,i++){
        /* this node will holds the value to add to the new sll */
        cairo_point *  temp = sll_get_data(pos, cairo_point);
        for(k = 0, temp_pos = pos->next; k < avg_windos - 1 && temp_pos != (*my_sll)->head ;k++, temp_pos = temp_pos->next){
            /* get che current node from the sll */
            cairo_point * curr_cairo_point = sll_get_data(temp_pos, cairo_point);
            temp->x += curr_cairo_point->x;
            temp->y += curr_cairo_point->y;
        }
        temp->x /= avg_windos;
        temp->y /= avg_windos;
        sll_append(smoothed_sll, temp);
    }
    
    /* swap the pointers */
    *temp_sll = *my_sll;
    *my_sll = smoothed_sll;
    
    sll_destroy(*temp_sll);

    
}