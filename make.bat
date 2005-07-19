@echo off

rem Command line parsing
:reparse
if "%1" == "clean" goto do_clean
if "%1" == "vc" goto build_makefiles
if not exist lib\Makefile.%1 goto usage


rem Build the library file
cmd /c cd lib ^& make.bat %*
if errorlevel 1 goto end
echo.
echo MySQL++ library built successfully.
echo.
copy lib\*.dll examples > NUL

cmd /c cd examples ^& make.bat %*
if errorlevel 1 goto end
echo.
echo MySQL++ examples built successfully.
echo.
goto end


rem Display usage message
:usage
echo usage: make {vc} [args]
echo.
echo     The first time you run this, you must give one of the compiler
echo     parameters as the first argument, so the script can build the
echo     proper Makefiles for your system.
echo.
echo     vc: build with Visual C++ command-line compiler and nmake
echo.
goto end


rem Remove generated files
:do_clean
if exist lib\Makefile cmd /c cd lib ^& make clean
del lib\Makefile lib\make.bat
if exist examples\Makefile cmd /c cd examples ^& make clean
del examples\Makefile examples\make.bat
shift
if "%1" NEQ "" goto reparse
goto end


rem Set up common headers for new Makefiles
:build_makefiles
set head1=## DO NOT EDIT.  Created from Makefile.%1 and Makefile.base.
set head2=## Edit those files instead, then type 'make' to rebuild
set head3=## this Makefile.

rem Decide how to build subordinate make.bat files
if "%1"=="vc" goto vc_make
goto generic_make
:vc_make
echo @nmake /nologo ^%* > lib\make.bat
echo @nmake /nologo ^%* > examples\make.bat
goto make_bat_created
:generic_make
echo @make.exe ^%* > lib\make.bat
echo @make.exe ^%* > examples\make.bat

:make_bat_created
rem Create library Makefile
echo %head1% > lib\Makefile
echo %head2% >> lib\Makefile
echo %head3% >> lib\Makefile
echo. >> lib\Makefile
echo. >> lib\Makefile
type lib\Makefile.%1 >> lib\Makefile
type lib\Makefile.base >> lib\Makefile

rem Create example programs Makefile
echo %head1% > examples\Makefile
echo %head2% >> examples\Makefile
echo %head3% >> examples\Makefile
echo. >> examples\Makefile
echo. >> examples\Makefile
type examples\Makefile.%1 >> examples\Makefile
type examples\Makefile.base >> examples\Makefile
shift
goto reparse


:end
