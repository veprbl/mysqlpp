/***********************************************************************
 stadapter.cpp - Implements the SQLTypeAdapter class.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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

#include "stadapter.h"

#include "stream2string.h"

#include <iomanip>
#include <sstream>

using namespace std;

namespace mysqlpp {

SQLTypeAdapter::SQLTypeAdapter() :
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(const string& str) :
string(str),
is_string_(true),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(const Null<string>& str) :
string(str.is_null ? null_str : str.data),
is_string_(!str.is_null),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(const char* str) : 
string(str),
is_string_(true),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(const char* str, size_t len) : 
string(str, len),
is_string_(true),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(char c) :
string(stream2string(c)),
is_string_(true),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<char> c) :
string(c.is_null ? null_str : stream2string(c)),
is_string_(true),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(sql_tinyint i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<sql_tinyint> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(sql_tinyint_unsigned i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<sql_tinyint_unsigned> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(short i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLTypeAdapter::SQLTypeAdapter(Null<short> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(unsigned short i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<unsigned short> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(int i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<int> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(unsigned int i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<unsigned int> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(longlong i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<longlong> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(ulonglong i) :
string(stream2string(i)),
is_string_(false),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(Null<ulonglong> i) :
string(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(float f) :
is_string_(false),
is_processed_(false)
{
	ostringstream outs;
	outs.precision(7);	// max digits in IEEE 754 single-prec float
	outs << f;
	assign(outs.str());
}

SQLTypeAdapter::SQLTypeAdapter(Null<float> f) :
is_string_(false),
is_processed_(false)
{
	if (f.is_null) {
		assign(null_str);
	}
	else {
		ostringstream outs;
		outs.precision(7);	// as above
		outs << f;
		assign(outs.str());
	}
}

SQLTypeAdapter::SQLTypeAdapter(double f) :
is_string_(false),
is_processed_(false)
{
	ostringstream outs;
	outs.precision(16);	// max digits in IEEE 754 double-prec float
	outs << f;
	assign(outs.str());
}

SQLTypeAdapter::SQLTypeAdapter(Null<double> f) :
is_string_(false),
is_processed_(false)
{
	if (f.is_null) {
		assign(null_str);
	}
	else {
		ostringstream outs;
		outs.precision(16);	// as above
		outs << f;
		assign(outs.str());
	}
}

SQLTypeAdapter::SQLTypeAdapter(const Date& d) :
string(stream2string(d)),
is_string_(true),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(const Null<Date>& d) :
string(d.is_null ? null_str : stream2string(d)),
is_string_(true),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(const DateTime& dt) :
string(stream2string(dt)),
is_string_(true),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(const Null<DateTime>& dt) :
string(dt.is_null ? null_str : stream2string(dt)),
is_string_(true),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(const Time& t) :
string(stream2string(t)),
is_string_(true),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(const Null<Time>& t) :
string(t.is_null ? null_str : stream2string(t)),
is_string_(true),
is_processed_(false) 
{
}

SQLTypeAdapter::SQLTypeAdapter(const null_type& i) :
string(null_str),
is_string_(false),
is_processed_(false)
{
}

} // end namespace mysqlpp

