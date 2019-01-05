#-----------------------------------------------------
# Target file to be compiled by default
#-----------------------------------------------------
MAIN = main
#-----------------------------------------------------
# CC is the compiler to be used
#-----------------------------------------------------
CC = gcc
#-----------------------------------------------------
# Directories of source and object files
#-----------------------------------------------------
ODIR = ./obj
SDIR = ./src
#-----------------------------------------------------
# CFLAGS are the options passed to the compiler
#-----------------------------------------------------
CFLAGS = -Wall -I./headers -L./lib
#-----------------------------------------------------
# OBJS are the object files to be linked
#-----------------------------------------------------
OBJ1 = init
OBJS = $(patsubst %, $(ODIR)/%, $(MAIN).o $(OBJ1).o)
#-----------------------------------------------------
# LIBS are the external libraries to be used
#-----------------------------------------------------
LIBS = -lptask -lpthread -lrt -lm `allegro-config --libs`
#-----------------------------------------------------
# Dependencies
#-----------------------------------------------------
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

$(ODIR)/$(MAIN).o: $(MAIN).c
	$(CC) -c $(CFLAGS) -o $@ $< $(LIBS)

$(ODIR)/$(OBJ1).o: $(SDIR)/$(OBJ1).c
	$(CC) -c $(CFLAGS) -o $@ $< $(LIBS)

#-----------------------------------------------------
# Command that can be specified inline: make all
#-----------------------------------------------------
all: $(MAIN)
#-----------------------------------------------------
# Command that can be specified inline: make clean
#-----------------------------------------------------
.PHONY: clean
clean:
	rm -rf $(ODIR)/* *.o $(MAIN)
#-----------------------------------------------------
