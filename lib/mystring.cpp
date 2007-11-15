/***********************************************************************
 mystring.cpp - Implements the String class.

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

#include "mystring.h"

#include <stdexcept>
#include <string>

namespace mysqlpp {


String::~String()
{
	dec_ref_count();
}


char
String::at(size_type pos) const
{
	if (pos >= size()) {
		throw std::out_of_range("String");
	}
	else {
		return buffer_->data()[pos];
	}
}


int
String::compare(const String& other) const
{
	if (buffer_) {
		if (other.buffer_) {
			const char* ptb = buffer_->data();			// ptr to this buffer
			const char* pob = other.buffer_->data();	// ptr to other buffer
			const size_type short_len = std::min(length(), other.length());
			for (size_type i = 0; i < short_len; ++i) {
				if (ptb[i] != pob[i]) {
					return ptb[i] - pob[i];
				}
			}
			
			return length() - other.length();
		}
		else {
			// Arbitrarily consider a String that has a buffer to be
			// "greater than" one that is default-constructed.
			return 1;
		}
	}
	else if (other.buffer_) {
		// Reverse of above rule
		return -1;
	}
	else {
		// Neither String has a buffer, so consider them to be equal.
		return 0;
	}
}


template <>
String
String::conv(String dummy) const { return *this; }


template <>
std::string
String::conv(std::string dummy) const
		{ return std::string(data(), length()); }


template <>
Date
String::conv(Date dummy) const { return Date(c_str()); }


template <>
DateTime
String::conv(DateTime dummy) const { return DateTime(c_str()); }


template <>
Time
String::conv(Time dummy) const { return Time(c_str()); }


const char*
String::data() const
{
	return buffer_ ? buffer_->data() : 0;
}


String::const_iterator
String::end() const
{
	return buffer_ ? buffer_->data() + buffer_->length() : 0;
}


bool
String::escape_q() const
{
	return buffer_ ? buffer_->type().escape_q() : false;
}


bool
String::is_null() const
{
	return buffer_ ? buffer_->is_null() : false;
}


void
String::it_is_null()
{
	if (buffer_) {
		buffer_->set_null();
	}
	else {
		buffer_ = new RefCountedBuffer(0, 0, mysql_type_info::string_type, true);
	}
}


String::size_type
String::length() const
{
	return buffer_ ? buffer_->length() : 0;
}


bool
String::quote_q() const
{
	return buffer_ ? buffer_->type().quote_q() : false;
}


void
String::to_string(std::string& s) const
{
	if (buffer_) {
		s.assign(buffer_->data(), buffer_->length());
	}
	else {
		s.clear();
	}
}


char
String::operator [](size_type pos) const
{
	return buffer_ ? buffer_->data()[pos] : 0;
}


} // end namespace mysqlpp
