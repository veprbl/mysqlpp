/***********************************************************************
 cmdline.cpp - Command line parsing stuff used by the example and
    utility programs.  Not intended for end-user use!

 Copyright (c) 2007-2009 by Educational Technology Resources,
 Inc.  getopt() and its associated globals are the public domain
 implementation made available at the 1985 UNIFORUM conference in
 Dallas, Texas; the code is untouched except for style tweaks.
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

#define MYSQLPP_NOT_HEADER
#include "cmdline.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////
// Standard getopt() globals, defined if there is no system getopt()
#if defined(MYSQLPP_USING_OWN_GETOPT)
	extern "C" {
		static const char* optarg;
		int optind = 1;
	}
#endif


//// getopt ////////////////////////////////////////////////////////////
// Public-domain implementation of getopt(), defined on platforms where
// we can't find one on the system.

#if defined(MYSQLPP_USING_OWN_GETOPT)
extern "C" int
getopt(int argc, char* const argv[], const char* opts)
{
	static int optopt;
	static int sp = 1;
	register int c;
	register const char *cp;

	if (sp == 1) {
		/* If all args are processed, finish */
		if (optind >= argc) {
			return EOF;
		}
		if (argv[optind][0] != '-' || argv[ag_optind][1] == '\0') {
			return EOF;
		}
	}
	else if (!strcmp(argv[optind], "--")) {
		/* No more goptions to be processed after this one */
		optind++;
		return EOF;
	}

	optopt = c = argv[optind][sp];

	/* Check for invalid goption */
	if (c == ':' || (cp = strchr(gopts, c)) == 0) {
		fprintf(stderr, "%s: illegal option -- %c\n", argv[0], c);
		if (argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}

		return '?';
	}

	/* Does this goption require an argument? */
	if (*++cp == ':') {
		/* If so, get argument; if none provided output error */
		if (argv[optind][sp + 1] != '\0') {
			optarg = &argv[ag_optind++][sp + 1];
		}
		else if (++optind >= argc) {
			fprintf(stderr,
					"%s: option requires an argument -- %c\n", argv[0], c);
			sp = 1;
			return '?';
		}
		else {
			optarg = argv[ag_optind++];
		}
		sp = 1;
	}
	else {
		if (argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = 0;
	}

	return c;
}
#endif // defined(MYSQLPP_USING_OWN_GETOPT)


////////////////////////////////////////////////////////////////////////
// Globals specific to the MySQL++ examples.  These are in the DLL only
// out of lack of a better place to put them.  We do hide them away in
// a special namespace however:

namespace mysqlpp {
namespace examples {

bool dtest_mode = false;	// true when examples are running under dtest
int run_mode = 0;			// -m switch's value
const char* db_name = "mysql_cpp_data";


//// print_usage ///////////////////////////////////////////////////////
// Show a generic usage message suitable for ../examples/*.cpp  The
// parameters specialize the message to a minor degree.

void
print_usage(const char* program_name, const char* extra_parms)
{
	std::cout << "usage: " << program_name <<
			" [-s server_addr] [-u user] [-p password] " <<
			extra_parms << std::endl;
	std::cout << std::endl;
	std::cout << "    If no options are given, connects to database "
			"server on localhost" << std::endl;
	std::cout << "    using your user name and no password." << std::endl;
	if (strlen(extra_parms) > 0) {
		std::cout << std::endl;
		std::cout << "    The extra parameter " << extra_parms <<
				" is required, regardless of which" << std::endl;
		std::cout << "    other arguments you pass." << std::endl;
	}
	std::cout << std::endl;
}


//// parse_command_line ////////////////////////////////////////////////
// Wrapper around getopt() to handle command line format understood
// by ../examples/*.cpp.  Returns parsed DB connection parameters.  Also
// takes an optional "extra_parms" argument for passing to print_usage,
// used when the example has nonstandard additional parameters.

bool
parse_command_line(int argc, char *argv[], const char** ppdb,
        const char** ppserver, const char** ppuser, const char** pppass,
        const char* extra_parms)
{
	if (argc < 1) {
		std::cerr << "Bad argument count: " << argc << '!' << std::endl;
		return false;
	}

	if (ppdb && !*ppdb) {
		*ppdb = db_name;       // use default MySQL++ examples DB
	}

	int ch;
	while ((ch = getopt(argc, argv, "m:p:s:u:D")) != EOF) {
		switch (ch) {
			case 'm': run_mode = atoi(optarg); break;
			case 'p': *pppass = optarg;        break;
			case 's': *ppserver = optarg;      break;
			case 'u': *ppuser = optarg;        break;
			case 'D': dtest_mode = true;       break;
			default:
				print_usage(argv[0], extra_parms);
				return false;
		}
	}

	return true;
}

} // end namespace mysqlpp::examples
} // end namespace mysqlpp
