TESTDEF=-DCALLOC_TEST
CC=gcc
CFLAGS=-Wall -O3
all: yetanotherht.exe

yetanotherht.exe: yetanotherht.o yaht.o
	$(CC) $(CFLAGS) $? -o $@

test: CFLAGS += $(TESTDEF)
test: yetanotherht.exe

yetanotherht.o: yetanotherht.c yaht.h yaht.inl.h
	$(CC) $(CFLAGS) -c $< -o $@

yaht.o: yaht.c yaht.h yaht.inl.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o
