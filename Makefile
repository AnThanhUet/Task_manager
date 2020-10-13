PRO_DIR     := .
OUTPUT_PATH := $(PRO_DIR)/output
INFO_SYS_C  := $(PRO_DIR)/info_sys/info_sys.c
TEST_C	    := $(PRO_DIR)/test/test_proc.c
INFO_SYS_H  := $(PRO_DIR)/info_sys/info_sys.h
TEST_H      := $(PRO_DIR)/test/test_proc.h

CC = gcc
CFLAGS = -I.

.PHONY: main info_sys.o main.o test_proc.o
%.o: %.c $(INFO_SYS_H) $(TEST_H)
	$(CC) -c -o $@ $^ $(CFLAGS) 

main: main.o info_sys.o test_proc.o
	$(CC) -o $@ $^ $(CFLAGS) -pthread

info_sys.o: $(INFO_SYS_C)
	$(CC) -c $< $(CFLAGS)

test_proc.o: $(TEST_C)
	$(CC) -c $< $(CFLAGS)  

main.o: main.c
	$(CC) -c $< $(CFLAGS)

clean:
	@rm main main.o info_sys.o test_proc.o
