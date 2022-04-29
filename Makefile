CC = clang

C_STD_FLAG = -std=c99

GCC_FLAGS = -Wall -Wextra -Werror 
CLANG_FLAGS = -Weverything
DEBUG_FLAG = -g
LDFLAGS = -pthread


ifeq ($(CC),gcc)
	CFLAGS=$(GCC_FLAGS)
else
	CFLAGS=$(CLANG_FLAGS)
endif


CUT: main.o queue.o reader.o analyzer.o printer.o
	$(CC) $(STD_FLAG) $(CFLAGS) $(DEBUG_FLAG) $(LDFLAGS) -o $(@) $(^)

.PHONY: clean
clean:
	rm -f *.o CUT