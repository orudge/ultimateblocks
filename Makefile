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
EXE_NOEXT= blocks4
endif


ifeq ($(PLATFORM),mingw )
CFLAGS   += -DALLEGRO_STATICLINK -DENABLE_CDA
LDFLAGS  += -lcda -laldmb -ldumb -lalleg_s -lwinmm -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -ldsound -Wl,-subsystem,windows
RES      = res.o
EXE      = blocks4.exe
#SVNVERSION = $(shell svnversion -n)
#SVNVERSION_NUM = $(shell echo $(SVNVERSION) | sed "s/[^0-9]//g")

ifeq ($(shell if test -d .svn; then echo 1; fi), 1)
# Find if the local source if modified
REV_MODIFIED := $(shell svnversion $(SRC_DIR) | sed -n 's/.*\(M\).*/\1/p' )
# Find the revision like: rXXXX-branch
SVNVERSION := $(shell LC_ALL=C svn info . | awk '/^URL:.*branch/ { split($$2, a, "/"); BRANCH="-"a[5] } /^Last Changed Rev:/ { REV="r"$$4"$(REV_MODIFIED)" } END { print REV BRANCH }')
SVNVERSION_NUM := $(shell LC_ALL=C svn info . | awk '/^Last Changed Rev:/ { print $$4 }')
else
SVNVERSION := 0
SVNVERSION_NUM := 0
endif

endif

ifeq ($(PLATFORM),macosx )
CC       = ./gcc-uni.sh
CFLAGS   += -DALLEGRO_STATICLINK -DMACOSX
LDFLAGS  += -laldmb -ldumb `allegro-config --static`
RES      =
EXE      = blocks4
endif

ifeq ($(PLATFORM),linux )
CC       = gcc
CFLAGS   += -DENABLE_CDA
LDFLAGS  += -laldmb -ldumb `allegro-config --libs` -lcda
RES      =
EXE      = blocks4
endif

WINDRES = windres

RM = rm -f

OBJECTS = bomb.o   dos.o    editor.o  fps.o   init.o   main.o   motif.o  ply.o \
          box.o    fall.o   laser.o   menu.o  music.o  sound.o  undo.o   trans.o \
          door.o   gfx.o    levels.o  mon.o   part.o   title.o  vars.o   win.o $(RES)

all: $(EXE)
	@echo "Ultimate Blocks has been built."

$(EXE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

ifeq ($(PLATFORM),djgpp )
	exe2coff $(EXE)
	copy /b cwsdstub.exe+$(EXE_NOEXT) $(EXE)
	$(RM) $(EXE_NOEXT)
endif

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

res_rev.tmp :
	$(RM) res_rev.tmp
	echo "#define SVN_VERSION \"SVN revision $(SVNVERSION)\"" > res_rev.tmp
	echo "#define SVN_FILEVERSION 4,0,0,$(SVNVERSION_NUM)" >> res_rev.tmp

res.o : blocks4.rc
	echo "#define SVN_VERSION \"SVN revision $(SVNVERSION)\"" > res_rev.tmp
	echo "#define SVN_FILEVERSION 4,0,0,$(SVNVERSION_NUM)" >> res_rev.tmp
	$(WINDRES) $< $@ -DWITH_REV
	rm -f res_rev.tmp

clean:
	$(RM) *.o *~

vclean: clean
	$(RM) $(EXE)
