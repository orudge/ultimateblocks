-include makefile.cfg

null: $(PLATFORM)

ifeq ($(PLATFORM),)
	$(error Please run configure.bat or configure before make)
endif

ifeq ($(PLATFORM),undefined)
	$(error Please run configure.bat or configure before make)
endif

undefined :

djgpp : all
mingw : all
linux : all
macosx : all

ifeq ($(DEBUG),1)
CFLAGS   = -g
CPPFLAGS = -g
LDFLAGS  =
else
CFLAGS   = -s
CPPFLAGS = -s
LDFLAGS  =
endif

ifeq ($(PLATFORM),djgpp )
LDFLAGS  += -lcda -laldmb -ldumb -lalleg
RES      =
endif


ifeq ($(PLATFORM),mingw )
CFLAGS   += -DALLEGRO_STATICLINK
LDFLAGS  += -lcda -laldmb -ldumb -lalleg_s -lwinmm -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -ldsound
RES      = res.o
endif

ifeq ($(PLATFORM),macosx )
CFLAGS   += -DALLEGRO_STATICLINK
LDFLAGS  += -laldmb -ldumb `allegro-config --libs`
RES      =
endif

CC = gcc
WINDRES = windres

RM = rm -f

OBJECTS = bomb.o   editor.o init.o   main.o   motif.o  ply.o    trans.o \
box.o    fall.o   laser.o  menu.o   music.o  sound.o  undo.o \
door.o   gfx.o    levels.o mon.o    part.o   title.o $(RES)

all: blocks4

blocks4: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

res.o : blocks4.rc
	$(WINDRES) $< $@

clean:
	$(RM) *.o *~

vclean: clean
	$(RM) blocks4
