//
//  draw_helper.c
//  EX2_GRADED_SP_CAIROCHARTS
//
//  Created by Francesco  on 04/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#include "draw_helper.h"
typedef struct{
    float scale_x;
    float scale_y;
    float height;
    float min_pos_y;
    float width;
    float arrow_space_x;
    float arrow_space_y;

    cairo_point * origin;
    
} window;
void set_parameters(cairocharts_payload *, cairo_t *);
void destroy_cairocharts(cairo_surface_t *, cairo_t *);
/* This function draws axis AND arrows */
void draw_axis(cairocharts_payload * my_payload, cairo_t *cr, window *,int);
cairo_point * get_origin(cairocharts_payload *,float,float,window *);
void draw_line_plot(cairocharts_payload *, sll *, cairo_t * ,window * );
void draw_histogram(cairocharts_payload *, sll *, cairo_t * , window *);

void draw_lines(cairocharts_payload *, cairo_t *, float,float, float,window *);

void get_max(sll *my_sll, float * x, float * y){
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
    
    *x = max_x;
    *y = max_y;
    
}

void get_min(sll *my_sll, float * x, float * y){
    sll_node *pos;
    float min_x, min_y;
    
    min_x = 0.0;
    min_y = 0.0;
    
    for(pos = my_sll->head->next; pos != my_sll->head; pos = pos->next){
        
        cairo_point * curr_cairo_point = sll_get_data(pos, cairo_point);
        
        if(min_x > curr_cairo_point->x)
            min_x = curr_cairo_point->x;
        if(min_y > curr_cairo_point->y)
            min_y = curr_cairo_point->y;
    }
    
    *x = min_x;
    *y = min_y;
    
}

int create_cairocharts(cairocharts_payload * my_payload, sll *float_std_sll){
    cairo_surface_t *surface;
    cairo_t *cr;
    float max_x,max_y,min_x,min_y;
    window * my_window;
    int there_are_negative_y;
    
    there_are_negative_y= 0;
    my_window = malloc(sizeof(window));
    if(!my_window)
        return 0;
    surface = cairo_pdf_surface_create(my_payload->output,my_payload->width, my_payload->height);
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        printf("Error creating cairo surface\n");
        return 0;
    }
    
    cr = cairo_create (surface);
    if(!cr)
        return 0;
    
    set_parameters(my_payload,cr);
    get_max(float_std_sll,&max_x,&max_y);
    get_min(float_std_sll,&min_x,&min_y);
    if(my_payload->type == HISTOGRAM)
        max_x++;
    my_window->width = my_payload->width - (2*(my_payload->ymargin)) ;
    my_window->arrow_space_x = my_window->width/10.0;
    my_window->width -= my_window->arrow_space_x;

    
    my_window->height = my_payload->height - (2*(my_payload->xmargin));
    my_window->arrow_space_y = my_window->height/10.0;
    my_window->height -= my_window->arrow_space_y;
    
    my_window->min_pos_y = my_payload->height - (my_payload->xmargin);

    there_are_negative_y = min_y < 0.0;
    
    if(there_are_negative_y){
        my_window->min_pos_y -=(my_window->arrow_space_y);
        my_window->height -= my_window->arrow_space_y;

    }
    
    if(max_x == 0)
        max_x = 1;
    
    my_window->scale_x = (my_window->width )/max_x;
    my_window->scale_y = (my_window->height)/(max_y - min_y);
    my_window->origin = get_origin(my_payload,max_y,min_y,my_window);
    
    if(my_payload->type == HISTOGRAM){
        draw_histogram(my_payload,float_std_sll,cr,my_window);
    }
    else{
        /* we don't care if it is LINEPLOT or XPLOT
         since the scaling is done in the same way */
        draw_line_plot(my_payload,float_std_sll,cr,my_window);
    }
#ifdef DEBUG
    printf("Max x: %0.2f, Max y: %0.2f \n",max_x,max_y);
    printf("Min x: %0.2f, Min y: %0.2f \n",min_x,min_y);
    printf("Origin (x,y) (%0.2f,%0.2f) \n",my_window->origin->x,my_window->origin->y);
    
    /* We need a scale factor in order to plot */
    printf("Real width %0.2f, height %0.2f\n",my_window->width,my_window->height);
    printf("Scaled by a factor of (%0.2f,%0.2f)\n",my_window->scale_x,my_window->scale_y);
    printf("window MIN POS %0.2f\n",my_window->min_pos_y);
    printf("arrow x, arrow y %0.2f, %0.2f\n",my_window->arrow_space_x,my_window->arrow_space_y);

    
#endif
    
    draw_axis(my_payload,cr,my_window, there_are_negative_y);
    
    draw_lines(my_payload,cr,max_x,max_y, min_y,my_window);
    
    cairo_show_page(cr);
    
    destroy_cairocharts(surface, cr);
    return 1;
}

void normalize_point(cairo_point * curr_cairo_point, cairocharts_payload * my_payload, float scale_x, float scale_y){
    
    curr_cairo_point->x = (curr_cairo_point->x * scale_x);
    curr_cairo_point->y = (curr_cairo_point->y * scale_y);
}
void draw_histogram(cairocharts_payload * my_payload, sll *my_sll, cairo_t *cr,window * my_window){
    cairo_point * curr_cairo_point;
    sll_node *pos;
    
    
    cairo_set_line_width (cr, my_payload->linewidth * 2);
    cairo_set_source_rgb (cr, my_payload->color[0], my_payload->color[1], my_payload->color[2]);
    
    cairo_new_path(cr);
    
    
    for(pos = my_sll->head->next; pos != my_sll->head; pos = pos->next){
        
        curr_cairo_point = sll_get_data(pos, cairo_point);
        normalize_point(curr_cairo_point,my_payload,my_window->scale_x,my_window->scale_y);
        cairo_move_to (cr, curr_cairo_point->x + my_window->origin->x, my_window->origin->y);
        /* up line */
        cairo_line_to (cr, curr_cairo_point->x +my_window->origin->x, my_window->origin->y  - curr_cairo_point->y);
        /* right line */
        cairo_line_to (cr, curr_cairo_point->x +  my_window->scale_x +my_window->origin->x, my_window->origin->y  - curr_cairo_point->y);
        /* bottom line */
        cairo_line_to (cr, curr_cairo_point->x +  my_window->scale_x + my_window->origin->x,my_window->origin->y);
    }
    
    cairo_stroke (cr);
    
}

void abs_value(float * point){
    *point = *point < 0? *point * -1: *point;
}

void draw_line_plot(cairocharts_payload * my_payload, sll *my_sll, cairo_t *cr, window * my_window){
    cairo_point * curr_cairo_point;
    sll_node *pos;
    
    
    cairo_set_line_width (cr, my_payload->linewidth * 2);
    cairo_new_path(cr);
    
    
    for(pos =  my_sll->head->next; pos != my_sll->head; pos = pos->next){
        
        curr_cairo_point = sll_get_data(pos, cairo_point);
        normalize_point(curr_cairo_point,my_payload,my_window->scale_x,my_window->scale_y);
        cairo_line_to (cr, curr_cairo_point->x + my_window->origin->x ,my_window->origin->y - curr_cairo_point->y);
        
    }
    
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_stroke (cr);
    
}

void draw_axis(cairocharts_payload * my_payload, cairo_t *cr, window * my_window, int there_are_negative_y){
    float len_line;
    
    len_line = my_window->width/my_window->height * 2.0;
    
    cairo_set_line_width (cr, my_payload->linewidth);
    cairo_set_source_rgb (cr, 0, 0, 0);
    
    /* X axis */
    cairo_move_to (cr, my_window->origin->x - (my_payload->linewidth /2.0), my_window->origin->y);
    /* x is normalized by adding half of the linewidth, in that way the have a
     perfect match with the y axis */
    cairo_line_to (cr, my_window->origin->x /*+ (my_payload->linewidth /2)*/, my_window->origin->y);
    cairo_line_to (cr, my_window->origin->x + my_window->width + my_window->arrow_space_x /*+ (my_payload->linewidth /2)*/, my_window->origin->y);
    
    /* draw arrow on X axis */
    cairo_line_to (cr, my_window->origin->x - len_line + my_window->width + my_window->arrow_space_x ,  my_window->origin->y  + len_line);
    cairo_move_to (cr, my_window->origin->x + my_window->width + my_window->arrow_space_x , my_window->origin->y);
    cairo_line_to (cr, my_window->origin->x - len_line + my_window->width + my_window->arrow_space_x ,  my_window->origin->y  - len_line);
    
    /* Y axis */
    cairo_move_to (cr, my_window->origin->x, my_window->origin->y);
    cairo_line_to (cr, my_window->origin->x, my_payload->xmargin );
    /* draw arrow on Y axis */
    cairo_line_to (cr, my_window->origin->x +len_line ,  (0 + my_payload->xmargin)  + len_line);
    cairo_move_to (cr, my_window->origin->x , 0 + my_payload->xmargin );
    cairo_line_to (cr, my_window->origin->x - len_line ,  (0 + my_payload->xmargin) + len_line);
    
    if(there_are_negative_y){
        cairo_move_to (cr, my_window->origin->x, my_window->origin->y);
        cairo_line_to (cr, my_window->origin->x, my_payload->height - my_payload->xmargin );
        cairo_move_to (cr, my_window->origin->x , (my_payload->height - my_payload->xmargin) );
        cairo_line_to (cr, my_window->origin->x +len_line ,  (my_payload->height - my_payload->xmargin)  - len_line);
        
        cairo_move_to (cr, my_window->origin->x , (my_payload->height - my_payload->xmargin) );
        cairo_line_to (cr, my_window->origin->x - len_line ,  (my_payload->height - my_payload->xmargin) - len_line);
    }
    cairo_stroke (cr);
    
}

void set_to_origin(cairo_point *, cairo_point *);

void draw_lines(cairocharts_payload * my_payload , cairo_t * cr, float max_x, float max_y, float min_y,window * my_window){
    float distance_on_x,distance_on_y, step_on_x, step_on_y,len_line, show_pos;
    int i;
    char to_show[30];
    cairo_point * curr_pos;
    
    curr_pos = malloc(sizeof(cairo_point));
    /* Adjust the distance between ticks */
    distance_on_x = (my_window->width)/10.0;
    distance_on_y = (my_window->height)/10.0;
    
    step_on_x = 0.0;
    step_on_y = 0.0;
    len_line = my_window->width/my_window->height * 2.0;
    
    set_to_origin(my_window->origin,curr_pos);
    curr_pos->x -= (my_payload->linewidth /2.0)/2.0;
    
    cairo_set_line_width (cr, my_payload->linewidth/2.0);
#ifdef DEBUG
    printf("step,distance X (%0.2f,%0.2f) -- Y (%0.2f,%0.2f)\n",step_on_x,distance_on_x,step_on_y,distance_on_y);
#endif
    
    /* draw lines on X axis */
    for (i = 0; i<10; i++) {
        curr_pos->x += distance_on_x;
        step_on_x += distance_on_x/my_window->scale_x;
        cairo_move_to (cr, curr_pos->x,my_window->origin->y);
        cairo_line_to (cr, curr_pos->x , my_window->origin->y + len_line);
        /* convert from float to string */
        sprintf(to_show, "%.2f", step_on_x );
        
        show_pos = my_window->origin->y + len_line + my_payload->fontsize + 1;
        /* If show pos is negative it means that is outside the document, put it to 0 */
        show_pos = show_pos < 0 ? my_payload->height: show_pos;
        
        cairo_move_to (cr, curr_pos->x - ((my_payload->fontsize)/2.0),show_pos);
        cairo_show_text (cr, to_show);
        
    }

    curr_pos->x = my_window->origin->x;
    curr_pos->x -= (my_payload->linewidth /2.0)/2.0;
    curr_pos->y = my_window->min_pos_y;
    
    
    /* draw lines on Y axis */
    for (i = 0; i<11; i++) {
        cairo_move_to (cr, curr_pos->x,curr_pos->y);
        cairo_line_to (cr, curr_pos->x - len_line , curr_pos->y );
        step_on_y = (my_payload->height - curr_pos->y) * my_window->scale_y;
        
        snprintf(to_show, 16, "%0.1f", min_y + (max_y - min_y) / 10.0 * i);
        
        show_pos = my_window->origin->x - (len_line + ((strlen(to_show) *my_payload->fontsize)/2.0) + 1);
        show_pos = show_pos < 0 ? 0: show_pos;
        
        cairo_move_to (cr, show_pos, curr_pos->y  + my_payload->fontsize/2.0);
        cairo_show_text (cr, to_show);
        
        curr_pos->y -= distance_on_y;
        
    }
    
    
    cairo_stroke (cr);
    free(curr_pos);
    
}

void set_to_origin(cairo_point * origin, cairo_point * p){
    p->x = origin->x;
    p->y = origin->y;
    
}

cairo_point * get_origin(cairocharts_payload * my_payload, float max_y, float min_y,window * my_window){
    cairo_point * origin = malloc(sizeof(cairo_point));
    /* assume x can't be negative */
    origin->x = 0.0 + my_payload->ymargin;
    origin->y =my_window->min_pos_y + (my_window->scale_y * min_y);
    
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