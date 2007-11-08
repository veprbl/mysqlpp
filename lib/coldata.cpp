/***********************************************************************
 coldata.cpp - Implements the ColData class.

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

#include "coldata.h"

#include <stdexcept>
#include <string>

namespace mysqlpp {


ColData::~ColData()
{
	dec_ref_count();
}


char
ColData::at(size_type pos) const
{
	if (pos >= size()) {
		throw std::out_of_range("ColData");
	}
	else {
		return buffer_->data()[pos];
	}
}


int
ColData::compare(const ColData& other) const
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
			// Arbitrarily consider a ColData that has a buffer to be
			// "greater than" one that is default-constructed.
			return 1;
		}
	}
	else if (other.buffer_) {
		// Reverse of above rule
		return -1;
	}
	else {
		// Neither ColData has a buffer, so consider them to be equal.
		return 0;
	}
}


template <>
ColData
ColData::conv(ColData dummy) const { return *this; }


template <>
std::string
ColData::conv(std::string dummy) const
		{ return std::string(data(), length()); }


template <>
Date
ColData::conv(Date dummy) const { return Date(c_str()); }


template <>
DateTime
ColData::conv(DateTime dummy) const { return DateTime(c_str()); }


template <>
Time
ColData::conv(Time dummy) const { return Time(c_str()); }


const char*
ColData::data() const
{
	return buffer_ ? buffer_->data() : 0;
}


ColData::const_iterator
ColData::end() const
{
	return buffer_ ? buffer_->data() + buffer_->length() : 0;
}


bool
ColData::escape_q() const
{
	return buffer_ ? buffer_->type().escape_q() : false;
}


bool
ColData::is_null() const
{
	return buffer_ ? buffer_->is_null() : false;
}


void
ColData::it_is_null()
{
	if (buffer_) {
		buffer_->set_null();
	}
	else {
		buffer_ = new RefCountedBuffer(0, 0, mysql_type_info::string_type, true);
	}
}


ColData::size_type
ColData::length() const
{
	return buffer_ ? buffer_->length() : 0;
}


bool
ColData::quote_q() const
{
	return buffer_ ? buffer_->type().quote_q() : false;
}


void
ColData::to_string(std::string& s) const
{
	if (buffer_) {
		s.assign(buffer_->data(), buffer_->length());
	}
	else {
		s.clear();
	}
}


char
ColData::operator [](size_type pos) const
{
	return buffer_ ? buffer_->data()[pos] : 0;
}


} // end namespace mysqlpp
