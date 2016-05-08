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

void add_default_params(cairocharts_payload *);
int add_command_line_params(cairocharts_payload *, int, const char*[]);

cairocharts_payload * get_cairocharts_payload(int argc, const char * argv[]){
    cairocharts_payload * my_payload;
    
    if(!argv)
        return NULL;
    my_payload = malloc(sizeof(cairocharts_payload));
    
    if(!my_payload)
        return NULL;
    
    /* Add default parameters */
    add_default_params(my_payload);
    
    /* Add comand line parameters */
    if(!add_command_line_params(my_payload,argc,argv)){
        puts(InvalidComandLineArgument);
        return NULL;
    }
    
    return my_payload;
}


void cairocharts_destroy(cairocharts_payload *  this){
    free(this->output);
    free(this);
}


int parse_param(const char *, my_string**);
int store_and_parse_color(float *, char *);
void store_ouput(cairocharts_payload *, const char *);

int add_command_line_params(cairocharts_payload * my_payload, int argc, const char *argv[]){
    /* This will hold the current parameter parsed (e.g ["width","10.0"])*/
    my_string *curr_param[2];
    int error_code,i;
    
    curr_param[0] = my_string_init();
    curr_param[1] = my_string_init();

    error_code = 1;
    for(i = 1; i < argc; i++){
        
        if(!strchr(argv[i], '='))
            return 0;
        if(!parse_param(argv[i], curr_param))
            return 0;
        
        if(strcmp(curr_param[0]->string, "output") == 0){
            store_ouput(my_payload,curr_param[1]->string);
        }
        if(strcmp(curr_param[0]->string, "avg_window") == 0){
            if(sscanf(curr_param[1]->string,"%i",&my_payload->avg_window) == 0)
                error_code = 0;
        }
        if(strcmp(curr_param[0]->string, "width") == 0){
            if(sscanf(curr_param[1]->string,"%f",&my_payload->width) == 0)
                error_code = 0;
        }
        if(strcmp(curr_param[0]->string, "height") == 0){
            if(sscanf(curr_param[1]->string,"%f",&my_payload->height) == 0)
                error_code = 0;
        }
        if(strcmp(curr_param[0]->string, "xmargin") == 0){
            if(sscanf(curr_param[1]->string,"%f",&my_payload->xmargin) == 0)
                error_code = 0;
        }
        
        if(strcmp(curr_param[0]->string, "ymargin") == 0){
            if(sscanf(curr_param[1]->string,"%f",&my_payload->ymargin) == 0)
                error_code = 0;
        }
        if(strcmp(curr_param[0]->string, "fontsize") == 0){
            my_payload->fontsize = atof(curr_param[1]->string);
            if(sscanf(curr_param[1]->string,"%f",&my_payload->fontsize) == 0)
                error_code = 0;
        }
        
        if(strcmp(curr_param[0]->string, "linewidth") == 0){
            if(sscanf(curr_param[1]->string,"%f",&my_payload->linewidth) == 0)
                error_code = 0;

        }
        
        if(strcmp(curr_param[0]->string, "type") == 0){
            if(strcmp(curr_param[1]->string,"histogram") == 0)
               my_payload->type = HISTOGRAM;
            
            else if(strcmp(curr_param[1]->string,"xplot") == 0)
                my_payload->type = XPLOT;


        }

        if(strcmp(curr_param[0]->string, "color") == 0){
            if(!store_and_parse_color(my_payload->color,curr_param[1]->string))
                error_code = 0;
            
        }
    }
    
    my_string_destroy(curr_param[0]);
    my_string_destroy(curr_param[1]);
    
    return error_code;
}

void store_ouput(cairocharts_payload * my_payload, const char *output){
    my_string * output_my_string;
    
    output_my_string = my_string_init();
    
    my_string_copy_str(output_my_string, output);
    if(strstr(output, ".pdf") == NULL){
        my_string_add_str(output_my_string, ".pdf");
    }
        
    my_payload->output = malloc(output_my_string->size + 1);
    strcpy(my_payload->output, output_my_string->string);
    my_string_destroy(output_my_string);
}


int store_and_parse_color(float dst[], char *src){
    int i,error_code;
    char *token;
    float to_store;
    
    i = 0;
    error_code = 1;
    token = strtok(src, ",");
    while(token) {
        
        if(sscanf(token,"%f",&to_store) == 0)
            error_code = 0;
        else if(to_store > 1 || to_store < 0){
            puts("Color number must be between 0 and 1");
            error_code = 0;
        }
        dst[i] =  to_store;
        token = strtok(NULL, ",");
        i++;
    }
    return error_code;
}


void add_default_params(cairocharts_payload * my_payload){
    
    char * default_output = "chart.pdf";

    my_payload->output = strdup(default_output);
    
    my_payload->avg_window = 0;
    
    my_payload->type = LINE_PLOT;

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
    printf("%12s|%10s\n","Param","Value");
    puts("------------+-------------");
    
    printf("%12s|%10s\n","output",my_payload->output);
    printf("%12s|%10i\n","avg_windows",my_payload->avg_window);
    printf("%12s|%10.2f\n","width",my_payload->width);
    printf("%12s|%10.2f\n","height",my_payload->height);
    printf("%12s|%10.2f\n","xmargin",my_payload->xmargin);
    printf("%12s|%10.2f\n","ymargin",my_payload->ymargin);
    printf("%12s|%10.2f\n","fontsize",my_payload->fontsize);
    printf("%12s|%10.2f\n","linewidth",my_payload->linewidth);
    printf("%12s|%4.2f %3.2f %3.2f\n","color",my_payload->color[0],my_payload->color[1],my_payload->color[2]);
    
    
}

/* This function parse a argv element and store into the second paramam, eg:
 input "output=babbage" -> parsed into ["output","babbage"] */
int parse_param(const char * curr_argv, my_string ** dst){
    int error_code;
    char *temp_argv;
    
    error_code = 1;
    temp_argv = malloc(strlen(curr_argv) + 1);
    strcpy(temp_argv, curr_argv);
    
    if(!temp_argv)
        error_code = 0;

    if(my_string_copy_str(dst[0], strtok(temp_argv,"=")) == NULL)
        error_code = 0;
    if(my_string_copy_str(dst[1], strtok(NULL,"=")) == NULL)
        error_code = 0;

    return error_code;
}
