@echo on
bakefile -f msvs2005prj %* mysql++.bkl
start mysql++.sln
