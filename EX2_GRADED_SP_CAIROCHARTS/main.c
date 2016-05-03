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


int main(int argc, const char * argv[]) {
    cairocharts_payload * my_payload;
    sll *my_sll;
    
    my_sll = sll_init(sizeof(float));
    
    my_payload = get_cairochats_payload(argc,argv);
    print_payload(my_payload);
    
    return 0;
}
