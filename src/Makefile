CC = gcc   # This variable is which compiler to use, we will use the variable later by $(CC)
CFLAGS = -Wall  # this variable is command line arguments
CFILES = adj_list.c main.c utils.c  # this variable is the list of all the .c files to compile

all: main #runs target myprogram is nothing is passed into make

main: # it needs to compile out to >>>map.out<<<!
	$(CC) $(CFLAGS) -o map.out $(CFILES)  
	


clean: #this is a clean target, it removes all the .out files, called via > make clean
	rm  *.out