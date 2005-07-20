@echo off

rem Command line parsing
:reparse
if not exist lib\Makefile goto error
if "%1" == "clean" goto do_clean


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


rem Remove generated files
:do_clean
if exist lib\Makefile cmd /c cd lib ^& make clean
del lib\Makefile lib\make.bat
if exist examples\Makefile cmd /c cd examples ^& make clean
del examples\Makefile examples\make.bat
shift
if "%1" NEQ "" goto reparse
goto end


rem Display error message
:error
echo.
echo You must run makemake first!
goto end


:end
