//
//  draw_helper.h
//  EX2_GRADED_SP_CAIROCHARTS
//
//  Created by Francesco  on 04/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#ifndef draw_helper_h
#define draw_helper_h

#include <stdio.h>
#include <float.h>
#include "sentinel_linked_list.h"
#include "parse_helper.h"
#include <cairo.h>
#include <cairo-pdf.h>

typedef struct cairo_point{
    float x;
    float y;
} cairo_point;

int create_cairocharts(cairocharts_payload *, sll *);

#endif /* draw_helper_h */
