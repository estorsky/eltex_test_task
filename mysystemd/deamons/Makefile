CC = gcc
FLAGS = -pedantic -Wall -Wextra -O2 # -Werror
CFLAGS = -g
LIBS = -lmysystemd_shared # -lzmq -lpthread # -static
MYLIB = ../../mysystemd_shared_lib
SRC = exampled.c
EXE = exampled

.PHONY: all clean cp

all: $(EXE) cp

$(EXE):
	$(CC) $(SRC) -o $@ -L$(MYLIB) $(LIBS)

cp:
	cp $(EXE) $(EXE)_second
	cp $(EXE) $(EXE)_third

clean:
	rm -rf $(EXE) $(EXE)_second $(EXE)_third

# export LD_LIBRARY_PATH=/home/estor/test_task/mysystemd_shared_lib

