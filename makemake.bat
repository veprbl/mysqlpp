@echo off

rem Figure out which compiler the user wants to create Makefiles for
if "%1" == "vc" goto build_makefiles
if "%1" == "bc" goto build_makefiles
if "%1" == "mingw" goto build_makefiles
goto usage


:build_makefiles
rem Create Makefiles in subdirectories
for %d in ( lib examples ) do call buildmf.bat %1 %d

rem Create top-level Makefile
echo all clean install: > Makefile
echo 	cd lib ^&^& $(MAKE) $@ >> Makefile
echo 	cd examples ^&^& $(MAKE) $@ >> Makefile


rem Create release and debug subdirs, if they don't exist already
if not exist lib\debug mkdir lib\debug
if not exist lib\release mkdir lib\release


rem Start build process
shift
make %*
goto end


rem Display usage message
:usage
echo usage: makemake {vc, bc, mingw} [args]
echo.
echo     You must give one of the compiler parameters:
echo.
echo         vc: Visual C++ command-line compiler (cl)
echo         bc: Borland C++ command-line compiler (bcc32)
echo         mingw: MinGW GCC (g++)
echo.
echo     Any additional arguments you give will be passed on to the
echo     make program.
echo.
goto end


:end
