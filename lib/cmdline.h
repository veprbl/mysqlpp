/***********************************************************************
 cmdline.h - Declares the interface to the MySQL++'s command line
	parsing logic, used by the examples and the utility programs.
	Not intended for use by third parties!	If it breaks, you
	get to keep all the pieces.

 Copyright (c) 2007-2009 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS.txt file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#if !defined(MYSQLPP_CMDLINE_H) && !defined(DOXYGEN_IGNORE)
#define MYSQLPP_CMDLINE_H

#include "common.h"

// Figure out how to get definitions for getopt() and its associated
// globals.  Prefer POSIX version, fall back to GNU libiberty, and if
// all else fails, MySQL++ can provide one.
#if defined(HAVE_POSIX_GETOPT)
#	include <unistd.h>
#elif defined(HAVE_LIBIBERTY_GETOPT)
#	include <libiberty.h>
#else
#	define MYSQLPP_USING_OWN_GETOPT
	extern "C" int optind;
#endif

// MySQL++ specific stuff
namespace mysqlpp {
	// High-level parsing mechanism for ../examples/*.cpp
	namespace examples {
		extern bool dtest_mode;
		extern int run_mode;
		extern const char* db_name;
		extern bool parse_command_line(int argc, char *argv[],
				const char** ppdb, const char** ppserver,
				const char** ppuser, const char** pppass,
				const char* extra_parms = "");
		extern void print_usage(const char* program_name,
				const char* extra_parms = "");
	} // end namespace mysqlpp::examples
} // end namespace mysqlpp

#endif // !defined(MYSQLPP_CMDLINE_H)

