MinGW Version
~~~~~~~~~~~~~
    You need a fairly recent version of MinGW for MySQL++ to work.
    As I write this, the current release version includes GCC 3.4.2,
    which has known problems.  Unless they've released a newer version
    since I wrote this, you'll probably need to install the "candidate"
    version for best compatibility.


Prerequisite: Create Import Library
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Before you can build MySQL++ with MinGW, you will need to create
    a MinGW-compatible import library for MySQL's C API library.
    Using the current default install path for MySQL and assuming
    MySQL++ is in c:\mysql++, the commands to do this are:

        cd C:\Program Files\MySQL\MySQL Server 5.0\lib\opt
        dlltool -k -d c:\mysql++\libmysqlclient.def -l libmysqlclient.a


Prerequisite: MySQL C Development Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    You'll also need to have MySQL installed, including all the MySQL C
    development files; MySQL++ is built atop this library.  If it isn't
    in C:\Program Files\MySQL\MySQL Server 5.0\ you have two options.

    The simplest is to edit Makefile.mingw.  This is a generated
    file, but if that's all the only change to MySQL++ you need,
    it works fine.

    If you're doing deeper work on MySQL++, you should change the
    variable MYSQL_WIN_DIR at the top of mysql++.bkl instead.  Then to
    generate Makefile.mingw from that file, you will need the Win32
    port of Bakefile from http://bakefile.org/  The command to do
    that is:

        bakefile_gen -f mingw


Building the Library and Example Programs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Now you can build MySQL++ with this command:
    
        mingw32-make -f Makefile.mingw

    Notice that we're using the MinGW-specific version of GNU make,
    not the Cygwin or MSYS versions.  This is required, to get proper
    path separator handling.

    Once the library is built, you should run the examples.
    At minimum, run resetdb and simple1.

    Once you're satisfied that the library is working correctly, you
    can run the install.bat file at the project root to automatically
    install the library files and headers in subdirectories under
    c:\mysql++.
