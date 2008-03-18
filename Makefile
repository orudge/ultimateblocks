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
CFLAGS   = -s -O3
CPPFLAGS = -s -O3
LDFLAGS  =
endif

CC       = gcc

ifeq ($(PLATFORM),djgpp )
CFLAGS   += -DENABLE_CDA
LDFLAGS  += -lcda -laldmb -ldumb -lalleg
RES      =
EXE      = blocks4.exe
endif


ifeq ($(PLATFORM),mingw )
CFLAGS   += -DALLEGRO_STATICLINK -DENABLE_CDA
LDFLAGS  += -lcda -laldmb -ldumb -lalleg_s -lwinmm -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -ldsound
RES      = res.o
EXE      = blocks4.exe

SVNVERSION = $(shell svnversion -n)
endif

ifeq ($(PLATFORM),macosx )
CC       = ./gcc-uni.sh
CFLAGS   += -DALLEGRO_STATICLINK
LDFLAGS  += -laldmb -ldumb `allegro-config --static`
RES      =
EXE      = blocks4
endif

WINDRES = windres

RM = rm -f

OBJECTS = bomb.o   editor.o init.o   main.o   motif.o  ply.o    trans.o \
box.o    fall.o   laser.o  menu.o   music.o  sound.o  undo.o \
door.o   gfx.o    levels.o mon.o    part.o   title.o $(RES)

all: $(EXE)
	@echo "Ultimate Blocks has been built."

$(EXE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

res.o : blocks4.rc
	echo "#define SVN_VERSION \"SVN revision $(SVNVERSION)\"" > res_rev.tmp
	$(WINDRES) $< $@ -DWITH_REV
	rm -f res_rev.tmp

clean:
	$(RM) *.o *~

vclean: clean
	$(RM) $(EXE)
