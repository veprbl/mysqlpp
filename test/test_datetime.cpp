/***********************************************************************
 test_datetime.cpp - Tests the Date, DateTime, and Time classes.

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

#include <stdio.h>

using namespace mysqlpp;
using namespace std;


// Compare the given string against the object inserted into an ostream.
template <class T>
static unsigned int
test_ostream_form(const T& object, const char* expected, const char* what)
{
	ostringstream os;
	os << object;
	if (os.str().compare(expected) == 0) {
		return 0;
	}
	else {
		cerr << what << " '" << object << "' should be '" <<
				expected << "' in string form!" << endl;
		return 1;
	}
}


// Given a Date and a set of values we should expect to be in it,
// compare its outputs against values we compute separately.
static unsigned int
test_date(const Date& d, int year, int month, int day)
{
	if (d.year == year && d.month == d.month && d.day == day) {
		char ac[20];
		snprintf(ac, sizeof(ac), "%04d-%02d-%02d",
				year, month, day);
		return test_ostream_form(d, ac, "Date");
	}
	else {
		cerr << "Date '" << d << "' values should be '" <<
				year << '-' << month << '-' << day << endl;
		return 1;
	}
}


// Given a Time and a set of values we should expect to be in it,
// compare its outputs against values we compute separately.
static unsigned int
test_time(const Time& t, int hour, int minute, int second)
{
	if (t.hour == hour && t.minute == minute && t.second == second) {
		char ac[20];
		snprintf(ac, sizeof(ac), "%02d:%02d:%02d",
				hour, minute, second);
		return test_ostream_form(t, ac, "Time");
	}
	else {
		cerr << "Time '" << t << "' values should be '" <<
				hour << ':' << minute << ':' << second << endl;
		return 1;
	}
}


// Given a DateTime and a set of values we should expect to be in it,
// compare its outputs against values we compute separately.
static unsigned int
test_datetime(const DateTime& dt,
		int year, int month, int day,
		int hour, int minute, int second)
{
	return	test_date(Date(dt), year, month, day) +
			test_time(Time(dt), hour, minute, second);
}


// Run tests above for the various types we support using the date and
// time values given.
static unsigned int
test(int year, int month, int day, int hour, int minute, int second)
{
	unsigned int failures = 0;
	failures += test_date(Date(year, month, day), year, month, day);
	failures += test_datetime(
			DateTime(year, month, day, hour, minute, second),
			year, month, day, hour, minute, second);
	failures += test_time(Time(hour, minute, second), hour, minute,
			second);
	return failures;
}


int
main()
{
	unsigned int failures = 0;
	failures += test(0, 0, 0, 0, 0, 0);
	failures += test(1, 2, 3, 4, 5, 6);
	failures += test_ostream_form(DateTime(), "NOW()", "DateTime");
	DateTime dt;
	dt.year = 2007;
	failures += test_ostream_form(dt, "2007-00-00 00:00:00", "DateTime");
	return failures;
}

