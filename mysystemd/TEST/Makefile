CC = gcc
FLAGS = -pedantic -Wall -Wextra -O2 # -Werror
LIBS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lzlog
SRC  = $(wildcard *.c)
SRC += $(wildcard ../src/*.c)
SRC += $(wildcard ../src/*.h)
SRCTESTS = $(wildcard *.c)
TESTS = $(patsubst %.c, %, $(SRCTESTS))

.PHONY: all run clean

all: $(TESTS)

$(TESTS): $(SRC)
	$(CC) $@.c ../../unit_test/tap.c $(FLAGS) -o $@ $(LIBS)
	@echo "#######################################"
	@echo "start test $@"
	@./$@
	@echo "#######################################"

clean:
	rm -rf $(TESTS) 
	rm -rf *.log

