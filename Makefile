CC = clang

C_STD_FLAG = -std=c99

GCC_FLAGS = -Wall -Wextra  
CLANG_FLAGS = -Weverything
DEBUG_FLAG = -g
LDFLAGS = -pthread 


ifeq ($(CC),gcc)
	CFLAGS=$(GCC_FLAGS)
else
	CFLAGS=$(CLANG_FLAGS)
endif


CUT: main.o queue.o reader.o analyzer.o printer.o logger.o
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) $(^) -o $(@)
main.o: main.c
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -c $(^) -o $(@)
queue.o: queue.c
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -c $(^) -o $(@)
reader.o: reader.c
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -c $(^) -o $(@)
analyzer.o: analyzer.c
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -c $(^) -o $(@)
printer.o: printer.c
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -c $(^) -o $(@)
logger.o: logger.c
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -c $(^) -o $(@)


.PHONY: clean
clean:
	rm -f *.o