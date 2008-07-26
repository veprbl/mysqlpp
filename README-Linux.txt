For the most part, Linux is just another Unix variant as far as MySQL++
is concerned.  See README-Unix.txt for most of what you need to know
to build and use MySQL++.

The biggest Linux-specific hurdle is dealing with the dynamic linker,
ld.so.  This program is called by the operating system when loading
an executable to run.  It locates any shared libraries the executable
uses and links them up so the program will run.  If ld.so it can't
find libmysqlpp, a program using MySQL++ won't run.  It always looks
in /lib and /usr/lib for shared libraries, but by default, MySQL++
installs in /usr/local/lib when you install from source.  You can
either change the installation directory, or you can clue ld.so in
to the nonstandard location.

The first thing ld.so looks at when trying to find a library is the
LD_LIBRARY_PATH environment variable.  This works like the shell's
PATH variable: a colon-separated list of directories to search.
This option is best used when you can't use the next option, and
you can't install in /usr/lib.  (Or /lib, yes, but one would never
install a library like MySQL++ *there*, would one?)

The best way to tell ld.so about a nonstandard library directory is to
put it in /etc/ld.so.conf or in one of the files included from there.
Then, run ldconfig as root to rebuild the cache file ld.so uses to
find libraries in these nonstandard directories.
