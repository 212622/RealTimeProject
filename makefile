#---------------------------------------------------
# Target file to be compiled by default
#---------------------------------------------------
MAIN = prova
#---------------------------------------------------
# CC is the compiler to be used
#---------------------------------------------------
CC = gcc
#---------------------------------------------------
# CFLAGS are the options passed to the compiler
#---------------------------------------------------
CFLAGS = -Wall -I src/ -lpthread -lrt
#---------------------------------------------------
# OBJS are the object files to be linked
#---------------------------------------------------
OBJS = $(addsuffix .o, $(MAIN))
#---------------------------------------------------
# LIBS are the external libraries to be used
#---------------------------------------------------
LIBS = ./lib/libptask.a `pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_ttf-5`
#---------------------------------------------------
# Dependencies
#---------------------------------------------------
#%: %.c
#	$(CC) $< -o $@ $(LIBS) $(CFLAGS)

$(OBJS): $(MAIN).c
	$(CC) -o $(OBJS) $(MAIN).c $(LIBS) $(CFLAGS)

#---------------------------------------------------
# Command that can be specified inline: make all
#---------------------------------------------------
all: $(MAIN)
#---------------------------------------------------
# Command that can be specified inline: make clean
#---------------------------------------------------
.PHONY: clean
clean:
	rm -rf *.o $(MAIN)
