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

void free_memory(cairocharts_payload *, sll *);
/* This function takes the data from the stdio and it stores into the custom sentinel linked list */
int get_data_from_std(sll *);
void print_data(cairocharts_payload *, sll * );
int create_cairocharts(cairocharts_payload *, sll *);

void my_print(void *node){
    printf("%0.2f ",*sll_get_data(node,float));
}

int main(int argc, const char * argv[]) {
    /* This stuct will holds all the cmmd params */
    cairocharts_payload * my_payload;
    /* This sentinel linking list will holds the float points */
    sll *float_std_sll;
    
    float_std_sll = sll_init(sizeof(float));
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

int create_cairocharts(cairocharts_payload * my_payload, sll *float_std_sll){
    
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_pdf_surface_create(my_payload->output, my_payload->width, my_payload->height);
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        printf("Error creating cairo surface\n");
        return 0;
    }

    cr = cairo_create (surface);
    
    cairo_set_line_width (cr, 0.1);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_rectangle (cr, 0.25, 0.25, my_payload->width-0.5, my_payload->height-0.5);
    cairo_stroke (cr);
    
    cairo_show_page(cr);
    cairo_destroy(cr);
    cairo_surface_flush(surface);
    cairo_surface_destroy(surface);
    
    return 1;
    
}


void free_memory(cairocharts_payload * my_payload, sll* my_sll){
    cairocharts_destroy(my_payload);
    sll_destroy(my_sll);
}

void store_float_into_sll(sll *, my_string *);

int get_data_from_std(sll * my_sll){
    my_string * temp_string;
    char c;
    
    temp_string = my_string_init();
    
    if(!temp_string)
        return 0;
    
    for(;(c = getchar());){
        if(c == ' '){
            store_float_into_sll(my_sll,temp_string);
        }
        else if(c == '\n' || c == EOF){
            store_float_into_sll(my_sll,temp_string);
            break;
        }
        else{
            if(!my_string_add(temp_string, c))
                return 0;
        }
    }
    return 1;
}

void store_float_into_sll(sll * my_sll, my_string * temp_string){
    float temp_float;
    temp_float = atof(temp_string->string);
    sll_append(my_sll, &temp_float);
    /* reset the string */
    my_string_erase(temp_string);
}