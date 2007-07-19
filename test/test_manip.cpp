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
is_quoted(const std::string& s, T orig_str, size_t orig_len)
{
	return (s.length() == (orig_len + 2)) &&
			(s.at(0) == '\'') &&
			(s.at(orig_len + 1) == '\'') &&
			(s.compare(1, orig_len, orig_str) == 0);
}


// Stringish types should be quoted when inserted into Query when an
// explicit quote manipulator is used.
template <class T>
static bool
test_explicit_query_quote(mysqlpp::Query& q, T test, size_t len)
{
	q.reset();
	q << mysqlpp::quote << test;
	if (is_quoted(q.str(), test, len)) {
		return true;
	}
	else {
		std::cerr << "Explicit quote of " << typeid(test).name() <<
				" in Query failed: " << q.str() << std::endl;
		return false;
	}
}


// Stringish types should be quoted when inserted into an ostream when
// an explicit quote manipulator is used.
template <class T>
static bool
test_explicit_ostream_quote(T test, size_t len)
{
	std::ostringstream outs;
	outs << mysqlpp::quote << test;
	if (is_quoted(outs.str(), test, len)) {
		return true;
	}
	else {
		std::cerr << "Explicit quote of " << typeid(test).name() <<
				" in ostream failed: " << outs.str() << std::endl;
		return false;
	}
}


// Stringish types should be implicitly quoted when inserted into Query
template <class T>
static bool
test_implicit_query_quote(mysqlpp::Query& q, T test, size_t len)
{
	q.reset();
	q << test;
	if (is_quoted(q.str(), test, len)) {
		return true;
	}
	else {
		std::cerr << "Implicit quote of " << typeid(test).name() <<
				" in Query failed: " << q.str() << std::endl;
		return false;
	}
}


// Stringish types should NOT be implicitly quoted when inserted into
// non-Query ostreams
template <class T>
static bool
fail_implicit_ostream_quote(T test, size_t len)
{
	std::ostringstream outs;
	outs << mysqlpp::quote << test;
	if (!is_quoted(outs.str(), test, len)) {
		return true;
	}
	else {
		std::cerr << "Erroneously quoted " << typeid(test).name() <<
				" in non-Query ostream: " << outs.str() << std::endl;
		return false;
	}
}


// Run all tests above for the given type
template <class T>
static bool
test(mysqlpp::Query& q, T test, size_t len)
{
	return test_explicit_query_quote(q, test, len) &&
			test_explicit_ostream_quote(test, len) &&
			test_implicit_query_quote(q, test, len) &&
			!fail_implicit_ostream_quote(test, len);
}


int
main()
{
	mysqlpp::Connection c;
	mysqlpp::Query q(&c);

	char s[] = "Doodle me, James, doodle me!";
	const size_t len = strlen(s);

	int failures = 0;
	failures += test(q, s, len) == false;
	failures += test(q, (char*)s, len) == false;
	failures += test(q, (const char*)s, len) == false;
	failures += test(q, std::string(s), len) == false;
	failures += test(q, mysqlpp::ColData(s), len) == false;
	return failures;
}

