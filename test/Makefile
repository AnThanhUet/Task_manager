CC = gcc
CCFLAGS = -Wall

test: test.o
	$(CC) $(CCFLAGS) -o test test_proc.o -pthread

test.o: test_proc.c
	$(CC) $(CCFLAGS) -c test_proc.c -pthread

clean:
	- rm *.o
	- rm test
