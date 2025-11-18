CC = gcc   # This variable is which compiler to use, we will use the variable later by $(CC)
MAIN_CFLAGS = -Wall  # this variable is command line arguments
MAINPROGRAM = ./src/main.c ./src/utils.c ./src/debug.c ./src/GraphReader.c ./src/adjList.c ./src/NeuHashtable.c ./src/shortestPathService.c ./src/dijkstra.c # this variable is the list of all the .c files to compile
TESTPROGRAM = ./tests/test.c ./src/utils.c ./src/debug.c ./src/GraphReader.c ./src/adjList.c ./src/NeuHashtable.c ./src/shortestPathService.c ./src/dijkstra.c
TEST_CFLAGS = -Wall -I./src
all: myprogram #runs target myprogram is nothing is passed into make

main: # it needs to compile out to >>>map.out<<<!
	$(CC) $(MAIN_CFLAGS) -o map.out $(MAINPROGRAM)

myprogram: # it needs to compile out to >>>map.out<<<!
	$(CC) $(MAIN_CFLAGS) -o map.out $(MAINPROGRAM)  

test: 
	$(CC) $(TEST_CFLAGS) -o test.out $(TESTPROGRAM)

clean: #this is a clean target, it removes all the .out files, called via > make clean
	rm  *.out