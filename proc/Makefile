##INCLUDE_DIR=../lib
CFLAGS=-I$(INCLUDE_DIR) -Wall
##LDFLAGS=-L$(INCLUDE_DIR) -llcd_lib

TARGET=proc
CC=gcc

all:
	$(CC) proc.c -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(TARGET) 
