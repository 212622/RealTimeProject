#---------------------------------------------------------------
# Target file to be compiled by default
#---------------------------------------------------------------
MAIN = prova
#---------------------------------------------------------------
# CC will be the compiler to use
#---------------------------------------------------------------
CC = gcc
#---------------------------------------------------------------
# CFLAGS will be the options passed to the compiler
#---------------------------------------------------------------
CFLAGS = -Wall -I src/ -lpthread -lrt
#---------------------------------------------------------------
# CLIBS will be the libraries passed to the compiler
#---------------------------------------------------------------
CLIBS = ./lib/libptask.a `pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_ttf-5`
#---------------------------------------------------------------
# Dependencies
#---------------------------------------------------------------
all: $(MAIN)

.PHONY: clean
clean:
	rm -rf $(MAIN)

%: %.c
	$(CC) $< -o $@ $(CLIBS) $(CFLAGS)
