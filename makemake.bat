@echo off

BUILDSYS=vc

:build_makefiles
rem Create Makefiles in subdirectories
for %d in ( lib examples ) do call buildmf.bat %BUILDSYS% %d

rem Create top-level Makefile
echo all: > Makefile
echo 	cd lib ^&^& $(MAKE) BIN_DIR=debug >> Makefile
echo 	cd lib ^&^& $(MAKE) BIN_DIR=release >> Makefile
echo 	cd examples ^&^& $(MAKE) >> Makefile
echo. >> Makefile
echo install: >> Makefile
echo 	@cmd /c install.bat %BUILDSYS% >> Makefile
echo. >> Makefile
echo clean debug release: >> Makefile
echo 	cd lib ^&^& $(MAKE) $@ >> Makefile
echo 	cd examples ^&^& $(MAKE) $@ >> Makefile


rem Create release and debug subdirs, if they don't exist already
if not exist lib\debug mkdir lib\debug
if not exist lib\release mkdir lib\release
if not exist examples\debug mkdir examples\debug
if not exist examples\release mkdir examples\release


echo Created Makefiles successfully.
goto end


rem Display usage message
:usage
echo usage: makemake {vc, mingw} [args]
echo.
echo     You must give one of the compiler parameters:
echo.
echo         vc: Visual C++ command-line compiler (cl)
echo         mingw: MinGW GCC (g++)
goto end


:end
