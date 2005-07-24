#!/bin/sh

# Display usage message
function usage() {
	echo 'usage: makemake [simple] {gcc} [args]'
	echo
	echo '    You must give one of the compiler parameters:'
	echo
	echo '        gcc: GCC C++ compiler'
	echo
	echo 'If you give the "simple" argument, no top-level Makefile is created,'
	echo 'no config.h is created, and we don'"'"'t call "make" for you.'
	echo
	exit 1
}

# Strip comments from a Makefile
function strip_comments() {
	TMP=tmp`basename $1`
	sed -e '/^#/d' $1 |uniq > $TMP
	mv $TMP $1
}

# Create top-level Makefile and default config.h files if we're not in
# "simple" mode.
if [ $1 == "simple" ]
then
	SIMPLE=.simple
	shift
else
	cat > Makefile <<MAKEFILE
	all:
		( cd lib && make && cd ../examples && make )
MAKEFILE

	cat > config.h <<CONFIG_H
	#define HAVE_MYSQL_SHUTDOWN_LEVEL_ARG
CONFIG_H
fi

# Figure out which compiler the user wants to create Makefiles for
if [ "$1" != "gcc" ]
then
	usage
fi

# Set up common headers for new Makefiles
head1="## DO NOT EDIT.  Created from Makefile.$1 and Makefile.base."
head2="## Edit those files instead, then type 'makemake $1' to rebuild"
head3="## this Makefile."

# Create library Makefile
echo "$head1
$head2
$head3
" > lib/Makefile$SIMPLE
cat lib/Makefile.$1 lib/Makefile.base >> lib/Makefile$SIMPLE

# Create example programs Makefile
echo "$head1
$head2
$head3
" > examples/Makefile$SIMPLE
cat examples/Makefile.$1 examples/Makefile.base >> examples/Makefile$SIMPLE

if [ -n "$SIMPLE" ]
then
	# Strip comments out of Makefiles, to simplify them
	strip_comments lib/Makefile$SIMPLE
	strip_comments examples/Makefile$SIMPLE
else
	# We're not in "simple" mode, so start build process
	shift
	make $*
fi

