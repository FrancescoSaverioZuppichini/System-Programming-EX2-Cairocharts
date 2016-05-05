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
void draw_point(cairocharts_payload *, sll *, cairo_t * ,float ,float,cairo_point *);

void get_max(sll *my_sll, float * scale_x, float * scale_y){
    sll_node *pos;
    float max_x, max_y;
    
    max_x = 0.0;
    max_y = 0.0;
    
    for(pos = my_sll->head->next; pos != my_sll->head; pos = pos->next){
        
        cairo_point * curr_cairo_point = sll_get_data(pos, cairo_point);
        
        if(max_x < curr_cairo_point->x)
            max_x = curr_cairo_point->x;
        if(max_y < curr_cairo_point->y)
            max_y = curr_cairo_point->y;
    }
    
    *scale_x = max_x;
    *scale_y = max_y;
}

int create_cairocharts(cairocharts_payload * my_payload, sll *float_std_sll){
    cairo_surface_t *surface;
    cairo_t *cr;
    float scale_x, scale_y;
    
    surface = cairo_pdf_surface_create(my_payload->output,my_payload->width, my_payload->height);
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        printf("Error creating cairo surface\n");
        return 0;
    }
    cr = cairo_create (surface);
    if(!cr)
        return 0;
    
    set_parameters(my_payload,cr);
    get_max(float_std_sll,&scale_x,&scale_y);
    printf("Max x: %0.2f, Max y: %0.2f \n",scale_x,scale_y);
    /* We need a scale factor in order to plot */
    printf("Real width %0.2f, height %0.2f\n",my_payload->width - 2*(my_payload->ymargin),my_payload->height - 2*(my_payload->xmargin));
    scale_x = (my_payload->width - 2*(my_payload->ymargin))/scale_x;
    scale_y = (my_payload->height - 2*(my_payload->xmargin))/scale_y;
    
    printf("Scaled by a factor of (%0.2f,%0.2f)\n",scale_x,scale_y);
    
    cairo_set_font_size (cr,my_payload->fontsize);
    cairo_set_line_width (cr, my_payload->linewidth);
    
    cairo_point * origin = get_origin(my_payload);
    cairo_set_source_rgb (cr, 0, 0, 0);
    
    draw_axis(my_payload,origin,cr);
    draw_point(my_payload,float_std_sll,cr,scale_x,scale_y,origin);
    cairo_show_page(cr);
    destroy_cairocharts(surface, cr);
    return 1;
}

void normalize_point(cairo_point * curr_cairo_point, cairocharts_payload * my_payload, float scale_x, float scale_y){
    
    curr_cairo_point->x = (curr_cairo_point->x * scale_x)+ my_payload->ymargin;
    curr_cairo_point->y = (curr_cairo_point->y * scale_y) + my_payload->xmargin;
}

void draw_point(cairocharts_payload * my_payload, sll *my_sll, cairo_t *cr,float scale_x, float scale_y, cairo_point *origin){
    cairo_point * curr_cairo_point;
    sll_node *pos;
    pos = my_sll->head->next;
    
    curr_cairo_point = sll_get_data(pos, cairo_point);
    
    normalize_point(curr_cairo_point,my_payload,scale_x,scale_y);
    
    cairo_new_path(cr);
    
    cairo_move_to (cr, curr_cairo_point->x, my_payload->height - curr_cairo_point->y);
    for(pos = pos->next; pos != my_sll->head; pos = pos->next){
        
        curr_cairo_point = sll_get_data(pos, cairo_point);
        normalize_point(curr_cairo_point,my_payload,scale_x,scale_y);

        cairo_line_to (cr, curr_cairo_point->x, my_payload->height  - curr_cairo_point->y);

    }
    
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_stroke (cr);


}

void draw_axis(cairocharts_payload * my_payload, cairo_point * origin, cairo_t *cr){
    
    /* X axis */
    cairo_move_to (cr, origin->x - (my_payload->linewidth /2), origin->y);
    /* x is normalized by adding half of the linewidth, in that way the have a
     perfect match with the y axis */
    cairo_line_to (cr, my_payload->width-my_payload->ymargin /*+ (my_payload->linewidth /2)*/, origin->y);
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