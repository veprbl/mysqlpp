@echo on
bakefile_gen -f msvs2003prj %* && start mysql++.sln
