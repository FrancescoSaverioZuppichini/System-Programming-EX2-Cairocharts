#Makefile for Graded Assignment 2 System Programming
#by Francesco Saverio Zuppichini
CC=gcc
CAIROLIB = `pkg-config --cflags --libs cairo`
all: homework02_FrancescoSaverio_Zuppichini

homework02_FrancescoSaverio_Zuppichini: main.c my_string.c draw_helper.c sentinel_linked_list_int.c parse_helper.c
	$(CC) -o homework02_FrancescoSaverio_Zuppichini main.c my_string.c draw_helper.c sentinel_linked_list_int.c parse_helper.c $(CAIROLIB)


clean:
	rm ./homework02_FrancescoSaverio_Zuppichini
