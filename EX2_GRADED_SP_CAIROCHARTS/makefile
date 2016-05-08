#Makefile for Graded Assignment 2 System Programming
#by Francesco Saverio Zuppichini
CC=gcc
CAIROLIB = `pkg-config --cflags --libs cairo`
SOURCES =  main.c my_string.c draw_helper.c sentinel_linked_list.c parse_helper.c
TARGET = cairo_charts_generator
all: homework02_FrancescoSaverio_Zuppichini

homework02_FrancescoSaverio_Zuppichini: $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES) $(CAIROLIB)

clean:
	rm ./$(TARGET)

debug:
	$(CC) -o $(TARGET) $(SOURCES) $(CAIROLIB) -DDEBUG
