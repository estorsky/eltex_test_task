CC = gcc
FLAGS = -pedantic -Wall -Wextra -O2 # -Werror
CFLAGS = -g
LIBS = -lzmq -lpthread # -static
SRC = exampled.c
EXE = exampled

.PHONY: all clean

all: $(EXE)

$(EXE):
	$(CC) $(SRC) -o $@ $(LIBS) -L/home/estor/test_task/mysystemd_shared_lib -lmysystemd_shared

clean:
	rm -rf $(EXE)

#export LD_LIBRARY_PATH=/home/estor/test_task/mysystemd_shared_lib
