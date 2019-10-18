CC = gcc
FLAGS = -pedantic -Wall -Wextra -O2 # -Werror
CFLAGS = -g
LIBS = -lzlog -lzmq -lpthread # -static
SRCDIR = src
OBJDIR = obj
DEAMDIR = deamons
INCDIR = include
INC  = $(wildcard $(INCDIR)/*.h)
INC += $(wildcard $(SRCDIR)/*.h)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ =  $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = mysystemd

.PHONY: all clean

all: $(OBJDIR) $(DEAMDIR) $(EXE) tests

$(EXE): $(OBJ)
	$(CC) -L/usr/local/lib $(OBJ) -Wl,-rpath,/usr/local/lib -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC)
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEAMDIR):
	mkdir -p $(DEAMDIR)

tests:
	cd TEST; make all

clean:
	rm -rf $(OBJDIR)
	rm -rf $(EXE)
	cd TEST; make clean

