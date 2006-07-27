@echo off
set PROG=%1
if not exist %PROG% set PROG=debug\%1
shift

set PATH=..\lib;%PATH%
%PROG% %*
