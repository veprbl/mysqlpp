#!/bin/sh

# Display usage message
function usage() {
	echo 'usage: makemake {gcc} [args]'
	echo
	echo '    You must give one of the compiler parameters:'
	echo
	echo '        gcc: GCC C++ compiler'
	echo
	exit 1
}

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


" > lib/Makefile
cat lib/Makefile.$1 lib/Makefile.base >> lib/Makefile

# Create example programs Makefile
echo "$head1
$head2
$head3


" > examples/Makefile
cat examples/Makefile.$1 examples/Makefile.base >> examples/Makefile

# Create top-level Makefile
cat > Makefile <<MAKEFILE
all:
	( cd lib && make && cd ../examples && make )
MAKEFILE

# Create default config.h file
cat > config.h <<CONFIG_H
#define HAVE_MYSQL_SHUTDOWN_LEVEL_ARG
CONFIG_H
