/***********************************************************************
 ssx/main.cpp - Main driver module for ssqlsxlat, which does several
 	data translations related to the SSQLSv2 mechanism of MySQL++.  The
	primary one is SSQLSv2 language files (*.ssqls) to C++ source code,
	but there are others.  Run "ssqlsxlat -?" to get a complete list.

 Copyright (c) 2009 by Educational Technology Resources, Inc.
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

#include <cmdline.h>

#include <iostream>

using namespace std;


//// main //////////////////////////////////////////////////////////////

int
main(int argc, char* argv[])
{
	// Parse the command line
	mysqlpp::ssqlsxlat::CommandLine cmdline(argc, argv);
	if (cmdline) {
		return 0;
	}
	else {
		return 1;
	}
}

