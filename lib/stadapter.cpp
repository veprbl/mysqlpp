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

#include "mystring.h"
#include "refcounted.h"
#include "stream2string.h"

#include <iomanip>
#include <sstream>

using namespace std;

namespace mysqlpp {

SQLTypeAdapter::SQLTypeAdapter() :
is_processed_(false)
{
}

SQLTypeAdapter::SQLTypeAdapter(const SQLTypeAdapter& other) :
buffer_(other.buffer_),
is_processed_(false)
{
	buffer_->attach();
}

SQLTypeAdapter::SQLTypeAdapter(const String& other, bool processed) :
buffer_(other.buffer_),
is_processed_(processed)
{
	buffer_->attach();
}

SQLTypeAdapter::SQLTypeAdapter(const string& str, bool processed) :
buffer_(new RefCountedBuffer(str, mysql_type_info::string_type, false)),
is_processed_(processed)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<string>& str, bool processed) :
buffer_(new RefCountedBuffer(str.is_null ? null_str : str.data,
		str.is_null ? typeid(void) : typeid(str.data), str.is_null)),
is_processed_(processed)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const char* str, bool processed) : 
buffer_(new RefCountedBuffer(str, strlen(str),
		mysql_type_info::string_type, false)),
is_processed_(processed)
{
}

SQLTypeAdapter::SQLTypeAdapter(const char* str, int len, bool processed) : 
buffer_(new RefCountedBuffer(str, len, mysql_type_info::string_type, false)),
is_processed_(processed)
{
}

SQLTypeAdapter::SQLTypeAdapter(char c) :
buffer_(new RefCountedBuffer(stream2string(c),
		mysql_type_info::string_type, false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<char> c) :
buffer_(new RefCountedBuffer(c.is_null ? null_str : stream2string(c),
		c.is_null ? typeid(void) : typeid(c.data), c.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(sql_tinyint i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<sql_tinyint> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(sql_tinyint_unsigned i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<sql_tinyint_unsigned> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(short i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<short> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(unsigned short i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<unsigned short> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(int i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<int> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(unsigned i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<unsigned> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(longlong i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false)
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<longlong> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false)
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(ulonglong i) :
buffer_(new RefCountedBuffer(stream2string(i), typeid(i), false)),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<ulonglong> i) :
buffer_(new RefCountedBuffer(i.is_null ? null_str : stream2string(i),
		i.is_null ? typeid(void) : typeid(i.data), i.is_null)),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(float f) :
is_processed_(false)
{
	ostringstream outs;
	outs.precision(7);	// max digits in IEEE 754 single-prec float
	outs << f;
	buffer_ = new RefCountedBuffer(outs.str(), typeid(f), false);
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<float> f) :
is_processed_(false)
{
	if (f.is_null) {
		buffer_ = new RefCountedBuffer(null_str, typeid(void), true);
	}
	else {
		ostringstream outs;
		outs.precision(7);	// as above
		outs << f;
		buffer_ = new RefCountedBuffer(outs.str(), typeid(f.data), false);
	}
}
#endif

SQLTypeAdapter::SQLTypeAdapter(double f) :
is_processed_(false)
{
	ostringstream outs;
	outs.precision(16);	// max digits in IEEE 754 double-prec float
	outs << f;
	buffer_ = new RefCountedBuffer(outs.str(), typeid(f), false);
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(Null<double> f) :
is_processed_(false)
{
	if (f.is_null) {
		buffer_ = new RefCountedBuffer(null_str, typeid(void), true);
	}
	else {
		ostringstream outs;
		outs.precision(16);	// as above
		outs << f;
		buffer_ = new RefCountedBuffer(outs.str(), typeid(f.data), false);
	}
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const Date& d) :
buffer_(new RefCountedBuffer(stream2string(d), typeid(d), false)),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<Date>& d) :
buffer_(new RefCountedBuffer(d.is_null ? null_str : stream2string(d),
		d.is_null ? typeid(void) : typeid(d.data), d.is_null)),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const DateTime& dt) :
buffer_(new RefCountedBuffer(stream2string(dt), typeid(dt), false)),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<DateTime>& dt) :
buffer_(new RefCountedBuffer(dt.is_null ? null_str : stream2string(dt),
		dt.is_null ? typeid(void) : typeid(dt.data), dt.is_null)),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const Time& t) :
buffer_(new RefCountedBuffer(stream2string(t), typeid(t), false)),
is_processed_(false) 
{
}

#if !defined(DOXYGEN_IGNORE)
SQLTypeAdapter::SQLTypeAdapter(const Null<Time>& t) :
buffer_(new RefCountedBuffer(t.is_null ? null_str : stream2string(t),
		t.is_null ? typeid(void) : typeid(t.data), t.is_null)),
is_processed_(false) 
{
}
#endif

SQLTypeAdapter::SQLTypeAdapter(const null_type& n) :
buffer_(new RefCountedBuffer(null_str, typeid(void), true)),
is_processed_(false)
{
}

SQLTypeAdapter&
SQLTypeAdapter::assign(const SQLTypeAdapter& sta)
{
	dec_ref_count();
	buffer_ = sta.buffer_;
	buffer_->attach();
	is_processed_ = false;
	return *this;
}

SQLTypeAdapter&
SQLTypeAdapter::assign(const char* pc, int len)
{
	if (len < 0) {
		len = strlen(pc);
	}

	dec_ref_count();
	buffer_ = new RefCountedBuffer(pc, len,
			mysql_type_info::string_type, false);
	is_processed_ = false;
	return *this;
}

SQLTypeAdapter&
SQLTypeAdapter::assign(const null_type& n)
{ 
	dec_ref_count();
	buffer_ = new RefCountedBuffer(null_str, typeid(void), true);
	is_processed_ = false;
	return *this;
}

char
SQLTypeAdapter::at(size_type i) const throw(std::out_of_range)
{
	if (buffer_) {
		if (i <= length()) {
			return *(buffer_->data() + i);
		}
		else {
			throw out_of_range("Not enough chars in SQLTypeAdapter");
		}
	}
	else {
		throw out_of_range("SQLTypeAdapter buffer not initialized");
	}
}

int
SQLTypeAdapter::compare(const SQLTypeAdapter& other) const
{
	if (other.buffer_) {
		return compare(0, length(), other.buffer_->data());
	}
	else {
		return buffer_ ? 1 : 0;
	}
}

int
SQLTypeAdapter::compare(const string& other) const
{
	return compare(0, length(), other.data());
}

int
SQLTypeAdapter::compare(size_type pos, size_type num, string& other) const
{
	return compare(pos, num, other.data());
}

int
SQLTypeAdapter::compare(const char* other) const
{
	return compare(0, length(), other);
}

int
SQLTypeAdapter::compare(size_type pos, size_type num,
		const char* other) const
{
	if (buffer_ && other) {
		return strncmp(data() + pos, other, num);
	}
	else if (!other) {
		return 1;				// initted is "greater than" uninitted
	}
	else {
		return other ? -1 : 0;	// "less than" unless other also unitted
	}
}

const char*
SQLTypeAdapter::data() const
{
	return buffer_ ? buffer_->data() : 0;
}

void
SQLTypeAdapter::dec_ref_count()
{
	if (buffer_ && !buffer_->detach()) {
		delete buffer_;
		buffer_ = 0;
	}
}

SQLTypeAdapter::size_type
SQLTypeAdapter::length() const
{
	return buffer_ ? buffer_->length() : 0;
}

bool
SQLTypeAdapter::escape_q() const
{
	return buffer_ ? buffer_->escape_q() : false;
}

SQLTypeAdapter&
SQLTypeAdapter::operator =(const SQLTypeAdapter& rhs)
{
	return assign(rhs);
}

SQLTypeAdapter&
SQLTypeAdapter::operator =(const null_type& n)
{ 
	return assign(n);
}

bool
SQLTypeAdapter::quote_q() const
{
	return buffer_ ? buffer_->quote_q() : false;
}

int
SQLTypeAdapter::type_id() const
{
	return buffer_ ? buffer_->type().id() : 0;
}

} // end namespace mysqlpp

