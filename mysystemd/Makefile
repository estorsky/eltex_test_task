CC = gcc
FLAGS = -pedantic -Wall -Wextra -O2 # -Werror
CFLAGS = -g
LIBS = -lzlog -lzmq -lpthread # -static
SRCDIR = src
OBJDIR = obj
DEAMDIR = deamons
INCDIR = include
SRC = $(wildcard $(SRCDIR)/*.c)
INC = $(wildcard $(INCDIR)/*.h)
OBJ =  $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = mysystemd

.PHONY: all clean

all: $(OBJDIR) $(DEAMDIR) $(EXE)

$(EXE): $(OBJ)
	$(CC) -L/usr/local/lib $(OBJ) -Wl,-rpath,/usr/local/lib -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC)
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEAMDIR):
	mkdir -p $(DEAMDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(EXE)

