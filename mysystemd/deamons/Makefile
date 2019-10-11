include ../../Makefile.variable

CC = gcc
FLAGS = -pedantic -Wall -Wextra -O2 # -Werror
CFLAGS = -g
LIBS = -lmysystemd_shared # -static
SRC = exampled.c
EXE = exampled

.PHONY: all clean cp

all: $(EXE) cp

$(EXE):
	$(CC) -L$(MYSYSTEMD_SHARED_LIB_DIR) $(SRC) -Wl,-rpath,$(MYSYSTEMD_SHARED_LIB_DIR) -o $@ $(LIBS)

cp:
	cp $(EXE) $(EXE)_second
	cp $(EXE) $(EXE)_third

clean:
	rm -rf $(EXE) $(EXE)_second $(EXE)_third

