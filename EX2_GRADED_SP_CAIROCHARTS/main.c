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


int main(int argc, const char * argv[]) {
    
    cairocharts_payload * my_payload;
    
    my_payload = get_cairochats_payload(argc,argv);
    return 0;
}
