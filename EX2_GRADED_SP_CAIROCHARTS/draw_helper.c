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
void draw_lines(cairocharts_payload *,cairo_point *, cairo_t *, float,float,float,float);

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
    float scale_x, scale_y,max_x,max_y,real_width,real_height;
    
    surface = cairo_pdf_surface_create(my_payload->output,my_payload->width, my_payload->height);
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        printf("Error creating cairo surface\n");
        return 0;
    }
    cr = cairo_create (surface);
    if(!cr)
        return 0;
    
    real_width = my_payload->width - 2*(my_payload->ymargin);
    real_height = my_payload->height - 2*(my_payload->xmargin);
    
    set_parameters(my_payload,cr);
    get_max(float_std_sll,&max_x,&max_y);
    printf("Max x: %0.2f, Max y: %0.2f \n",max_x,max_y);
    /* We need a scale factor in order to plot */
    printf("Real width %0.2f, height %0.2f\n",real_width,real_height);
    scale_x = (real_width)/max_x;
    scale_y = (real_height)/max_y;
    
    printf("Scaled by a factor of (%0.2f,%0.2f)\n",scale_x,scale_y);
    
    cairo_set_font_size (cr,my_payload->fontsize);
    
    cairo_point * origin = get_origin(my_payload);
    
    draw_point(my_payload,float_std_sll,cr,scale_x,scale_y,origin);
    
    draw_axis(my_payload,origin,cr);
    
    draw_lines(my_payload,origin,cr,max_x,max_y,real_width,real_height);
    
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
    
    /* first node */
    pos = my_sll->head->next;
    
    curr_cairo_point = sll_get_data(pos, cairo_point);
    
    normalize_point(curr_cairo_point,my_payload,scale_x,scale_y);
    
    cairo_set_line_width (cr, my_payload->linewidth * 2);
    cairo_set_source_rgb (cr, my_payload->color[0], my_payload->color[1], my_payload->color[2]);

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
    cairo_set_line_width (cr, my_payload->linewidth);
    cairo_set_source_rgb (cr, 0, 0, 0);

    /* X axis */
    cairo_move_to (cr, origin->x - (my_payload->linewidth /2.0), origin->y);
    /* x is normalized by adding half of the linewidth, in that way the have a
     perfect match with the y axis */
    cairo_line_to (cr, my_payload->width-my_payload->ymargin /*+ (my_payload->linewidth /2)*/, origin->y);
    cairo_stroke (cr);
    /* Y axis */
    cairo_move_to (cr, origin->x, origin->y);
    cairo_line_to (cr, origin->x, 0 + my_payload->xmargin );
    
    cairo_stroke (cr);
    
}

void draw_lines(cairocharts_payload * my_payload ,cairo_point * origin, cairo_t * cr, float max_x, float max_y, float witdh, float height){
    float distance_on_x,distance_on_y, step_on_x, step_on_y,len_line;
    int i;
    char to_show[20];
    
    cairo_point * curr_pos;
    
    curr_pos = malloc(sizeof(cairo_point));
    
    distance_on_x = witdh/10.0;
    distance_on_y = height/10.0;
    
    step_on_x = max_x/10.0;
    step_on_y = max_y/10.0;
    len_line = witdh/height * 2.0;
    curr_pos->x = origin->x;
    curr_pos->y = origin->y;

    curr_pos->x -= (my_payload->linewidth /2.0)/2.0;
    cairo_set_line_width (cr, my_payload->linewidth/2.0);
    printf("step,distance X (%0.2f,%0.2f) -- Y (%0.2f,%0.2f)\n",step_on_x,distance_on_x,step_on_y,distance_on_y);
    cairo_move_to (cr, curr_pos->x,curr_pos->y);
    cairo_line_to (cr, curr_pos->x, curr_pos->y + len_line);
    sprintf(to_show, "%.2f", 0.0 );
    cairo_show_text (cr, to_show);
    /* draw lines on X axis */
    for (i = 0; i<10; i++) {
        curr_pos->x += distance_on_x;
        cairo_move_to (cr, curr_pos->x,origin->y);
        cairo_line_to (cr, curr_pos->x , origin->y + len_line);
        sprintf(to_show, "%.2f", step_on_x );
        cairo_show_text (cr, to_show);
        step_on_x += max_x/10.0;


    }
    curr_pos->x = origin->x;
    curr_pos->y = origin->y;
    curr_pos->y += (my_payload->linewidth /2.0)/2.0;
    
    /* draw lines on Y axis */
    for (i = 0; i<10; i++) {
        /* remember that 0.0 is at the top */
        curr_pos->y -= distance_on_y;
        cairo_move_to (cr, origin->x,curr_pos->y);
        cairo_line_to (cr, origin->x - len_line , curr_pos->y);
        sprintf(to_show, "%.2f", step_on_y );
        cairo_show_text (cr, to_show);
        step_on_y += max_y/10.0;
        
    }
    
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