/***********************************************************************
 test/string.cpp - Tests the behavior of mysqlpp::String, particularly
	its data conversion methods.

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


// Does an equality comparison on the value, forcing the string to
// convert itself to T on the way.  Note that we do this test in terms
// of greater and less than to avoid pedantic GCC warnings for the
// floating point type tests.
template <typename T>
static bool
test_equality(const mysqlpp::String& s, T value)
{
	T converted = s.conv(value);
	if ((value < converted) || (value > converted)) {
		std::cerr << "Type conversion to " << typeid(T).name() <<
				" failed: \"" << s << "\" != \"" << value << "\"." <<
				std::endl;
		return false;
	}
	else {
		return true;
	}
}


// Runs uses the above functions to test many different types of
// conversion.
static bool
test_numeric(const mysqlpp::String& s, int value)
{
	return	test_equality(s, static_cast<signed char>(value)) &&
			test_equality(s, static_cast<unsigned char>(value)) &&
			test_equality(s, static_cast<signed short>(value)) &&
			test_equality(s, static_cast<unsigned short>(value)) &&
			test_equality(s, static_cast<signed int>(value)) &&
			test_equality(s, static_cast<unsigned int>(value)) &&
			test_equality(s, static_cast<signed long>(value)) &&
			test_equality(s, static_cast<unsigned long>(value)) &&
#if !defined(NO_LONG_LONGS)
			test_equality(s, static_cast<mysqlpp::longlong>(value)) &&
			test_equality(s, static_cast<mysqlpp::ulonglong>(value)) &&
#endif
			test_equality(s, static_cast<float>(value)) &&
			test_equality(s, static_cast<double>(value));
}


static bool
test_quote_q(const mysqlpp::String& s, bool expected)
{
	if (s.quote_q() == expected) {
		return true;
	}
	else {
		std::cerr << s.type().name() << " should" <<
				(expected ? "" : " NOT") << " be quoted." << std::endl;
		return false;
	}
}


int
main(int, char* argv[])
{
	try {
		int failures = 0;
		mysqlpp::String empty;
		mysqlpp::String zero("0");
		mysqlpp::String nonzero("42");

		failures += test_equality(empty, mysqlpp::Date()) == false;
		failures += test_equality(empty, 
				mysqlpp::DateTime(0, 0, 0, 0, 0, 0)) == false;
		failures += test_equality(empty, mysqlpp::Time()) == false;
		failures += test_equality(empty, false) == false;
		failures += test_equality(nonzero, true) == false;
		failures += test_numeric(empty, 0) == false;
		failures += test_numeric(zero, 0) == false;
		failures += test_numeric(nonzero, 42) == false;
		failures += test_quote_q(empty, true) == false;
		failures += test_quote_q(mysqlpp::String("1", typeid(int)),
				false) == false;

		return failures;
	}
	catch (mysqlpp::Exception& e) {
		std::cerr << "Unexpected MySQL++ exception caught in " <<
				argv[0] << ": " << e.what() << std::endl;
		return 1;
	}
	catch (std::exception& e) {
		std::cerr << "Unexpected C++ exception caught in " <<
				argv[0] << ": " << e.what() << std::endl;
		return 1;
	}

}

