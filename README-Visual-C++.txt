Prerequisites
~~~~~~~~~~~~~
    You need to have the MySQL C API development files on your system,
    since MySQL++ is built on top of it.

    The easiest way to get it is to download Connector/C from
    mysql.com.

    If you need the MySQL server on your development system anyway,
    you you can choose to install the development files along with
    the server.  Some versions of the MySQL Server installer for
    Windows have installed the development files by default, while
    others have made it an optional install.


Project Files
~~~~~~~~~~~~~
    The distribution comes with three sets of .sln and .vcproj files
    in the vc2003, vc2005 and vc2008 subdirectories.

    We do this for several reasons:

      1. It lets you build MySQL++ with multiple versions of Visual
         C++ without the build products conflicting.

      2. For Visual C++ 2003, we had to disable the SSQLS feature
         because changes made in MySQL++ 3.0 now cause the compiler
         to crash while building.  See the Breakages chapter in the
         user manual for workarounds, if you must still use VC++ 2003.

      3. The VC++ 2005 and 2008 project files get built with both 32-
         and 64-bit targets enabled.  VC++ 2003 didn't have 64-bit
         compilers, so the project files for it have to be different
         for that reason alone.
         
         If you need only one CPU type, by the way, you can change
         this in Build > Configuration Manager.

    The Visual C++ 2008 project files should convert without problems
    in newer versions of Visual Studio.  The reason we don't ship such
    project files is simply that the tool we use to create them hasn't
    yet been updated to understand the new MSBuild based build system
    that Microsoft moved to with Visual Studio 2010.


Using Nonstandard MySQL Installations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    The Visual Studio project files that come with MySQL++ have
    everything set up correctly for the common case.  The biggest
    assumption in the settings is that you're building against the
    current stable version of Connector/C, which gets installed here
    at the time of this writing:

        C:\Program Files\MySQL\MySQL Connector C 6.0.2\

    If you installed a different version, or it's in a different
    directory, or you've installed the development files as part of
    MySQL Server on the same machine, you need to change the project
    files to reference the C API development files in that other
    location.  There are two ways to do this.

    The hard way is to make 16 different changes each to 44 separate
    project files.  If you're a talented Visual Studio driver,
    you can do this in as little as about 5 or 6 steps.  You might
    even get it right the first time.  If you are not so talented,
    you have to make all ~700 changes one at a time, and you almost
    certainly will *not* get it right the first time.

    The somewhat easier way is to open all these files in a text
    editor that lets you make a global search and replace on all
    open files.

    The easy way is to install Bakefile (http://bakefile.org/),
    change the value of the MYSQL_WIN_DIR variable near the top of
    mysql++.bkl in the top level of the MySQL++ source tree, and run
    rebake.bat.  This will rebuild all of the project files for you,
    using the new MySQL path in all the many places it's needed.


Building the Library and Example Programs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    You must build both the Debug and Release versions of the library,
    because a release build of your program won't work with a Debug
    version of the MySQL++ DLL.  These DLLs get different names, so
    you can install them in the same directory if needed: mysqlpp_d.dll
    for the Debug version, and mysqlpp.dll for the Release version.

    With the library built, run at least the resetdb and simple1
    examples to ensure that the library is working correctly.
    In addition to the other generic examples, there are a few
    Visual C++ specific examples that you might want to look at in
    examples\vstudio.  See README-examples.txt for further details.

    Once you're sure the library is working correctly, you can run
    the install.hta file at the project root to install the library
    files and headers in a directory of your choosing.
    
    (Aside: You may not have come across the .hta extension before.
    It's for a rarely-used feature of Microsoft's Internet Explorer,
    called HTML Applications.  Know what Adobe AIR is?  Kinda like
    that, only without the compilation into a single binary blob which
    you must install before you can run it.  Just open install.hta
    in a text editor to see how it works.)


Using MySQL++ in Your Own Projects
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    This is covered in the user manual, chapter 9.


Working With Bakefile
~~~~~~~~~~~~~~~~~~~~~
    MySQL++'s top-level Visual Studio project files aren't
    maintained directly.  Instead, we use a tool called Bakefile
    (http://bakefile.org/) to generate them from mysql++.bkl. Since
    there are so many project files in MySQL++, it's often simpler to
    edit this source file and "re-bake" the project files from it than
    to make your changes in Visual Studio.

    To do this, download the native Windows version of Bakefile from the
    web site given above.  Install it, and then put the installation
    directory in your Windows PATH.  Then, open up a command window, cd
    into the MySQL++ directory, and type "rebake".  This will run
    rebake.bat, which rebuilds the Visual Studio project files from
    mysql++.bkl.

    There's more information about using Bakefile in HACKERS.txt.


If You Run Into Problems...
~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Especially if you have linking problems, make sure your project
    settings match the above.  Visual C++ is very picky about things
    like run time library settings.  When in doubt, try running one
    of the example programs.  If it works, the problem is likely in
    your project settings, not in MySQL++.

