@echo off
if "%1"=="djgpp" goto djgpp
if "%1"=="mingw" goto mingw
if "%1"=="undefined" goto undefined

echo Usage: configure target
echo.
echo You did not specify a platform to configure Ultimate Blocks for.
echo.
echo Possible targets:
echo.
echo     djgpp
echo     mingw
echo. 
echo Allegro, DUMB and aldumb must be installed on all platforms. Static libraries
echo will be used for MinGW target.
echo.
echo All options are case-sensitive.

goto realend

:djgpp
echo PLATFORM=djgpp > makefile.cfg
goto end

:mingw
echo PLATFORM=mingw > makefile.cfg
goto end

:undefined
echo PLATFORM=undefined> makefile.cfg
goto end

:end
echo Configuration complete. Now run `make'.

:realend