CC = gcc
CFLAGS = -s

RM = rm -f

OBJECTS = bomb.o   editor.o init.o   main.o   motif.o  ply.o    trans.o \
box.o    fall.o   laser.o  menu.o   music.o  sound.o  undo.o \
door.o   gfx.o    levels.o mon.o    part.o   title.o

all: blocks4

blocks4: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) -laldmb -ldumb `allegro-config --libs`

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o *~

vclean: clean
	$(RM) blocks4
