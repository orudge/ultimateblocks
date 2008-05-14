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
PLAT     = dos.o
EXE      = blocks4.exe
EXE_NOEXT= blocks4
APP      =
endif


ifeq ($(PLATFORM),mingw )
CFLAGS   += -DALLEGRO_STATICLINK -DENABLE_CDA
LDFLAGS  += -lcda -laldmb -ldumb -lalleg_s -lwinmm -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -ldsound -Wl,-subsystem,windows
RES      = res.o
PLAT     = win.o
EXE      = blocks4.exe
APP      =
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
PLAT     = osx.o
EXE      = blocks4
APP      = UltimateBlocks.app
endif

ifeq ($(PLATFORM),linux )
CC       = gcc
CFLAGS   += -DENABLE_CDA
LDFLAGS  += -laldmb -ldumb `allegro-config --libs` -lcda
RES      =
PLAT     = unix.o
EXE      = blocks4
APP
endif

WINDRES = windres

RM = rm -f

OBJECTS = bomb.o   editor.o  fps.o   init.o   main.o   motif.o  ply.o \
          box.o    fall.o   laser.o   menu.o  music.o  sound.o  undo.o   trans.o \
          door.o   gfx.o    levels.o  mon.o   part.o   title.o  vars.o   $(RES) $(PLAT)

all: $(EXE) $(APP)
	@echo "Ultimate Blocks has been built."

$(EXE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

ifeq ($(PLATFORM),djgpp )
	exe2coff $(EXE)
	copy /b os\dos\cwsdstub.exe+$(EXE_NOEXT) $(EXE)
	$(RM) $(EXE_NOEXT)
endif

APPR = $(APP)/Contents/Resources

DATA_GFX = graphics/castle.bmp graphics/rocky.bmp graphics/snowy.bmp graphics/sunny.bmp \
           graphics/castle.bgp graphics/rocky.bgp graphics/snowy.bgp graphics/sunny.bgp

DATA_MUSIC = music/celesfnt.s3m music/exp4a.xm music/jungle.mod music/lounge.xm music/memories.xm \
             music/shopping.xm music/vivamine.mod

DATA_MAPS = maps/2blocks.map maps/b1.map maps/b2.map maps/blocks3.map

$(APP) : $(EXE)
	install -d $(APPR)/graphics $(APPR)/maps $(APPR)/music
	install -d $(APP)/Contents/MacOS
	install -C os/osx/Info.plist $(APP)/Contents
	install -C blocks4 $(APP)/Contents/MacOS
	install -C $(DATA_GFX) $(APPR)/graphics
	install -C $(DATA_MUSIC) $(APPR)/music
	install -C $(DATA_MAPS) $(APPR)/maps
	install os/osx/UltimateBlocks.icns $(APPR)/UltimateBlocks.icns

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.m
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
