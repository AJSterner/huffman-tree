
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -g -Wfatal-errors -pedantic

all: hofftree

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

hofftree: hofftree.o queue.o
	gcc $^ -o $@ 

testbitstream: testbitstream.o bitstream.o
	gcc $^ -o $@

testqueue: testqueue.o queue.o
	gcc $^ -o $@

clean:
	rm -f *.o hofftree