//
//  draw_helper.c
//  EX2_GRADED_SP_CAIROCHARTS
//
//  Created by Francesco  on 04/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#include "draw_helper.h"

void set_parameters(cairocharts_payload *, cairo_t *);
void destroy_cairocharts(cairo_surface_t *, cairo_t *);
void draw_axis(cairocharts_payload * my_payload, cairo_point * origin, cairo_t *cr);
cairo_point * get_origin(cairocharts_payload * );


int create_cairocharts(cairocharts_payload * my_payload, sll *float_std_sll){
    
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_pdf_surface_create(my_payload->output,my_payload->width, my_payload->height);
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        printf("Error creating cairo surface\n");
        return 0;
    }
    cr = cairo_create (surface);
    if(!cr)
        return 0;
    
    set_parameters(my_payload,cr);
    
    cairo_set_font_size (cr,my_payload->fontsize);
    cairo_set_line_width (cr, my_payload->linewidth);
    
    cairo_point * origin = get_origin(my_payload);
    cairo_set_source_rgb (cr, 0, 0, 0);
    
    draw_axis(my_payload,origin,cr);
    
    cairo_show_page(cr);
    destroy_cairocharts(surface, cr);
    return 1;
}

void draw_axis(cairocharts_payload * my_payload, cairo_point * origin, cairo_t *cr){
    
    /* X axis */
    cairo_move_to (cr, origin->x - (my_payload->linewidth /2), origin->y);
    /* x is normalized by adding half of the linewidth, in that way the have a 
     perfect match with the y axis */
    cairo_line_to (cr, my_payload->width-my_payload->ymargin + (my_payload->linewidth /2), origin->y);
    cairo_stroke (cr);
    /* Y axis */
    cairo_move_to (cr, origin->x, origin->y);
    cairo_line_to (cr, origin->x, 0 + my_payload->xmargin );
    
    cairo_stroke (cr);

}

cairo_point * get_origin(cairocharts_payload * my_payload){
    cairo_point * origin = malloc(sizeof(cairo_point));
    
    origin->x = 0.0 + my_payload->ymargin;
    origin->y = my_payload->height - my_payload->xmargin;

    return origin;
}

void set_parameters(cairocharts_payload * my_payload, cairo_t *cr){
    cairo_set_font_size (cr,my_payload->fontsize);
    cairo_set_line_width (cr, my_payload->linewidth);
    cairo_set_source_rgb (cr, my_payload->color[0], my_payload->color[1], my_payload->color[2]);
    
}

void destroy_cairocharts(cairo_surface_t *surface, cairo_t *cr){
    cairo_destroy(cr);
    cairo_surface_flush(surface);
    cairo_surface_destroy(surface);
}