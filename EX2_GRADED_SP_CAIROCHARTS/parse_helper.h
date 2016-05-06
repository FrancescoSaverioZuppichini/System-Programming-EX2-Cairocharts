//
//  parse_helper.h
//  EX2_GRADED_SP_CAIROCHARTS
//
//  Created by Francesco  on 01/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#ifndef parse_helper_h
#define parse_helper_h
/* This module parse the command lines argument and returns a struct */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cairocharts_payload{
    /* filename output name (defaut: "chart.pdf") */
    char  * output;
    /* width of the all page in points (default: 4in)*/
    float width;
    /* height of the hole page in points (default: 3in)*/
    float height;
    /* left and right margins in points (default: 1/3in)*/
    float xmargin;
    /* top and buttom margins in points (default: 1/2in)*/
    float ymargin;
    /* font size in points (default: 8.0pt)*/
    float fontsize;
    /* basic line width for char axis in points; the plot line is 
     twice this line width (default: 1.0pt)*/
    float linewidth;
    /* RGB colors (default: 0.0,0.0,1.0) // blue */
    float color[3];
    /* Type of the graph, (default: line plot) */
    char *type;
}cairocharts_payload;

/* This function converts the command line parameters and return a struct
 with all the data in order to produce a chart if a errors occurs 
 a NULL pointer is returned.n */
cairocharts_payload * get_cairocharts_payload(int, const char *[]);
void print_payload(cairocharts_payload *);
void cairocharts_destroy(cairocharts_payload *);
#endif /* parse_helper_h */
