@echo on
del /q /f *.sln *.vcproj
bakefile -f msvc6prj %* mysql++.bkl
start mysql++.dsw
