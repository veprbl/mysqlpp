@echo on
bakefile_gen -f msvs2005prj %* && start mysql++.sln
