//
//  parse_helper.c
//  EX2_GRADED_SP_CAIROCHARTS
//
//  Created by Francesco  on 01/05/16.
//  Copyright © 2016 Francesco Saverio Zuppichini. All rights reserved.
//

#include "parse_helper.h"

#define PARAMS_LEN 8
#define data_delim "="
#define get_data(src)(strtok(src,data_delim))
#define InvalidComandLineArgument "Error during parsing command line arguments, exit program"
/* This function check if all the command line parameters exist. */
int is_argv_correct(int);
void add_default_params(cairocharts_payload *);
int add_command_line_params(cairocharts_payload *, int, char*[]);

cairocharts_payload * get_cairocharts_payload(int argc, const char * argv[]){
    cairocharts_payload * my_payload;
    
    if(!argv)
        return NULL;
    my_payload = malloc(sizeof(my_payload));
    
    if(!my_payload)
        return NULL;
    
    /* Add default parameters */
    add_default_params(my_payload);
    
    /* Add comand line parameters */
    if(!add_command_line_params(my_payload,argc,argv))
        return NULL;
    
    return my_payload;
}


void cairocharts_destroy(cairocharts_payload *  this){
    free(this->output);
    free(this);
}


void parse_param(char *, char*[]);
int store_and_parse_color(float *, char *);


int add_command_line_params(cairocharts_payload * my_payload, int argc, char *argv[]){
    /* This will hold the current parameter parsed (e.g ["width","10.0"])*/
    char *curr_param[2];
    
    int i;
    
    for(i = 1; i < argc; i++){
        
        parse_param(argv[i], curr_param);
        
        if(strcmp(curr_param[0], "output") == 0){
            my_payload->output = strdup(curr_param[1]);
        }
        if(strcmp(curr_param[0], "width") == 0){
            if(sscanf(curr_param[1],"%f",&my_payload->width) == 0)
                goto parsingError;
        }
        if(strcmp(curr_param[0], "height") == 0){
            if(sscanf(curr_param[1],"%f",&my_payload->height) == 0)
                goto parsingError;
        }
        if(strcmp(curr_param[0], "xmargin") == 0){
            if(sscanf(curr_param[1],"%f",&my_payload->xmargin) == 0)
                goto parsingError;
        }
        
        if(strcmp(curr_param[0], "ymargin") == 0){
            if(sscanf(curr_param[1],"%f",&my_payload->ymargin) == 0)
                goto parsingError;
        }
        if(strcmp(curr_param[0], "fontsize") == 0){
            my_payload->fontsize = atof(curr_param[1]);
            if(sscanf(curr_param[1],"%f",&my_payload->fontsize) == 0)
                goto parsingError;
        }
        
        if(strcmp(curr_param[0], "linewidth") == 0){
            if(sscanf(curr_param[1],"%f",&my_payload->linewidth) == 0)
                goto parsingError;

        }
        if(strcmp(curr_param[0], "color") == 0){
            if(!store_and_parse_color(my_payload->color,curr_param[1]))
                goto parsingError;
            
        }
    }
    
    return 1;
    parsingError:
        puts(InvalidComandLineArgument);
        return 0;
    
    
}

int store_and_parse_color(float dst[], char *src){
    int i;
    char *token;
    float to_store;
    
    i = 0;
    token = strtok(src, ",");
    while(token) {
        //        TODO add check here!
        if(sscanf(token,"%f",&to_store) == 0)
            return 0;

        else if(to_store > 1 || to_store < 0){
            puts("Color number must be between 0 and 1");
            return 0;
        }
        dst[i] =  to_store;
        token = strtok(NULL, ",");
        i++;
    }
    return 1;
}


void add_default_params(cairocharts_payload * my_payload){
    
    char * default_output = "chart.pdf";
    my_payload->output = strdup(default_output);
    
    my_payload->width = 4.0 * 72;
    
    my_payload->height = 3.0 * 72;
    
    my_payload->xmargin = (1.0/3.0) * 72;
    
    my_payload->ymargin = (1.0/2.0) * 72;
    
    my_payload->fontsize = 8.0;
    
    my_payload->linewidth = 1.0;
    
    my_payload->color[0] = 0.0;
    my_payload->color[1] = 0.0;
    my_payload->color[2] = 1.0;
    
}

void print_payload(cairocharts_payload * my_payload){
    printf("%10s|%10s\n","Param","Value");
    puts("----------+-------------");
    
    printf("%10s|%10s\n","output",my_payload->output);
    printf("%10s|%10.2f\n","width",my_payload->width);
    printf("%10s|%10.2f\n","height",my_payload->height);
    printf("%10s|%10.2f\n","xmargin",my_payload->xmargin);
    printf("%10s|%10.2f\n","ymargin",my_payload->ymargin);
    printf("%10s|%10.2f\n","fontsize",my_payload->fontsize);
    printf("%10s|%10.2f\n","linewidth",my_payload->linewidth);
    printf("%10s|%4.2f %3.2f %3.2f\n","color",my_payload->color[0],my_payload->color[1],my_payload->color[2]);
    
    
}

/* This function parse a argv element and store into the second paramam, eg:
 input "output=babbage" -> parsed into ["output","babbage"] */
void parse_param(char * curr_argv, char * dst[2]){
    
    dst[0] = strdup(strtok(curr_argv,"="));
    dst[1] = strdup(strtok(NULL,"="));
    
}


int is_argv_correct(int argc){
    return argc > 9;
}