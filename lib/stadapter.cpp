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

SQLTypeAdapter::SQLTypeAdapter(const string& str, bool processed) :
buffer_(str),
is_string_(true),
is_processed_(processed)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<string>& str, bool processed) :
buffer_(str.is_null ? null_str : str.data),
is_string_(!str.is_null),
is_processed_(processed)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const char* str, bool processed) : 
buffer_(str),
is_string_(true),
is_processed_(processed)
{
}

SQLTypeAdapter::SQLTypeAdapter(const char* str, int len, bool processed) : 
buffer_(str, len),
is_string_(true),
is_processed_(processed)
{
}

SQLTypeAdapter::SQLTypeAdapter(char c) :
buffer_(stream2string(c)),
is_string_(true),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<char> c) :
buffer_(c.is_null ? null_str : stream2string(c)),
is_string_(true),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(sql_tinyint i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<sql_tinyint> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(sql_tinyint_unsigned i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<sql_tinyint_unsigned> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(short i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
	ostringstream outs;
	outs << i;
	buffer_.assign(outs.str());
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<short> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(unsigned short i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<unsigned short> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(int i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<int> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(unsigned i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<unsigned> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(longlong i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<longlong> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(ulonglong i) :
buffer_(stream2string(i)),
is_string_(false),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<ulonglong> i) :
buffer_(i.is_null ? null_str : stream2string(i)),
is_string_(false),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(float f) :
is_string_(false),
is_processed_(false)
{
	ostringstream outs;
	outs.precision(7);	// max digits in IEEE 754 single-prec float
	outs << f;
	buffer_.assign(outs.str());
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<float> f) :
is_string_(false),
is_processed_(false)
{
	if (f.is_null) {
		buffer_.assign(null_str);
	}
	else {
		ostringstream outs;
		outs.precision(7);	// as above
		outs << f;
		buffer_.assign(outs.str());
	}
}
#endif

SQLTypeAdapter::SQLTypeAdapter(double f) :
is_string_(false),
is_processed_(false)
{
	ostringstream outs;
	outs.precision(16);	// max digits in IEEE 754 double-prec float
	outs << f;
	buffer_.assign(outs.str());
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<double> f) :
is_string_(false),
is_processed_(false)
{
	if (f.is_null) {
		buffer_.assign(null_str);
	}
	else {
		ostringstream outs;
		outs.precision(16);	// as above
		outs << f;
		buffer_.assign(outs.str());
	}
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const Date& d) :
buffer_(stream2string(d)),
is_string_(true),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<Date>& d) :
buffer_(d.is_null ? null_str : stream2string(d)),
is_string_(true),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const DateTime& dt) :
buffer_(stream2string(dt)),
is_string_(true),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<DateTime>& dt) :
buffer_(dt.is_null ? null_str : stream2string(dt)),
is_string_(true),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const Time& t) :
buffer_(stream2string(t)),
is_string_(true),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<Time>& t) :
buffer_(t.is_null ? null_str : stream2string(t)),
is_string_(true),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const null_type& i) :
buffer_(null_str),
is_string_(false),
is_processed_(false)
{
}

} // end namespace mysqlpp

