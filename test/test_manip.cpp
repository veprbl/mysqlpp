/***********************************************************************
 test_manip.cpp - Tests the quoting and escaping manipulators.

 Copyright (c) 2007 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS file in the top directory of the distribution for details.

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

#include <mysql++.h>

#include <iostream>
#include <sstream>
#include <string>


int
main()
{
	mysqlpp::Connection c;
	mysqlpp::Query q(&c);

	char test[] = "Doodle me, James, doodle me!";
	const size_t len = strlen(test);
	
	q << mysqlpp::quote << test;
	std::string result = q.str();
	if ((result.length() == (len + 2)) && (result[0] == '\'') &&
			(result[len + 1] == '\'')) {
		return 0;
	}
	else {
		std::cerr << "Failed to quote char[]" << std::endl;
		return 1;
	}
}

