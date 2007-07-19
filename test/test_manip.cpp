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


template <class T>
static bool
test_quote(mysqlpp::Query& q, T test, size_t len)
{
	q.reset();
	q << mysqlpp::quote << test;
	std::string result = q.str();
	if ((result.length() == (len + 2)) &&
			(result[0] == '\'') &&
			(result[len + 1] == '\'') &&
			(result.compare(1, len, test) == 0)) {
		return true;
	}
	else {
		std::cerr << "Failed to quote " << typeid(test).name() << std::endl;
		return false;
	}
}


int
main()
{
	mysqlpp::Connection c;
	mysqlpp::Query q(&c);

	char test[] = "Doodle me, James, doodle me!";
	const size_t len = strlen(test);
	if (test_quote(q, test, len) &&
			test_quote(q, (char*)test, len) &&
			test_quote(q, (const char*)test, len) &&
			test_quote(q, std::string(test), len)) {
		return 0;
	}
	else {
		return 1;
	}
}

