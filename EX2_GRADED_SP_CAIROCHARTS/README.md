#Cairo charts

##Graded ASsignment 2 - System Programming
### *Francesco Saverio Zuppichini*

###Project Structure
The project is composed by 5 source files and 4 headers:

- main.c
- my_string.c
- my_string.h
- sentinel\_linked\_list.c
- sentinel\_linked\_list.h
- parse_helper.c
- parse_helper.h
- draw_helper.c
- draw_helper.h

#### main.c
In this files the main function from the others source files are called. It creates a pointer to *generic sentinel linked list* called *float_std_sll* and to *cairocharts_payload* called *my_payload*. <br>
In detail *my_payload* is passed to *parse_helper.c*  that parse and store in it the command lines parameters, on the other and *cairo_point_sll*, as the name suggest, is used to store the *cairo\_point* struct collected from the standart input. All the std is stored into a *my_string* variable. This last parsing is done in *main.c* inside the function *store\_float\_into\_sll*. This function converts every float value from the std into *cairo\_point* that is defined in *draw\_helper* <br>
If the user select the *smoothing average* an addictional function is called, *smoothing_average*, that allocated a new *sentinel list* with the values of the smoothed point in *float\_std_sll* and then it changes the pointer to that list. <br>
After the two parsing are happened succesfully, *main.c* calls *create_cairocharts* from draw_helper.c in order to actually draw the graph.  <br>
In the end *free_memory* is called.

####parse_helper.c
The porpuse of this file is parsing all the data from the command lines parameters (argv). This is done by *create_cairocharts* that calls *add\_default\_params* and *add\_command\_line\_params*. <br>
The first function just add the default valus defined in your pdf, the second one parse and store into *my\_payload* all the argv. <br>
In order to do that an array of two *my_string* *curr_param* is allocated. It will holds the params name in the first elemenet and the value in the second. The color parsing is more trickly and a special function *store\_and\_parse\_color* is called.

####draw_helper.c
This is the main file, it draws the graph. This is done in differente step and they dipends on the grahp type. <br>
The main function is *create\_cairocharts* that select the correct draw function according to the type. <br>
A special enum *cairochart\_Type* defined into *parse\_helper.c*  is used to store the type
####Line plot
