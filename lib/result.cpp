/***********************************************************************
 result.cpp - Implements the Result, ResNSel, and ResUse classes.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in the
 top directory of the distribution for details.

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

#include "result.h"

#include "dbdriver.h"

namespace mysqlpp {

ResUse::ResUse(MYSQL_RES* result, DBDriver* dbd, bool te) :
OptionalExceptions(te),
driver_(result ? dbd : 0),
fields_(this)
{
	if (result) {
		result_ = result;
		names_ = new FieldNames(this);
		types_ = new FieldTypes(this);
	}
}


ResUse::~ResUse()
{
}


void
ResUse::copy(const ResUse& other)
{
	set_exceptions(other.throw_exceptions());

	if (other.result_) {
		result_ = other.result_;
		fields_ = Fields(this);
		names_ = other.names_;
		types_ = other.types_;
		driver_ = other.driver_;
	}
	else {
		result_ = 0;
		names_ = 0;
		types_ = 0;
		driver_ = 0;
	}
}


void
Result::data_seek(ulonglong offset) const
{
	driver_->data_seek(result_.raw(), offset);
}


const unsigned long*
ResUse::fetch_lengths() const
{
	return driver_->fetch_lengths(result_.raw());
}


Row
ResUse::fetch_row() const
{
	if (!result_) {
		if (throw_exceptions()) {
			throw UseQueryError("Results not fetched");
		}
		else {
			return Row();
		}
	}
	MYSQL_ROW row = driver_->fetch_row(result_.raw());
	const unsigned long* lengths = fetch_lengths();
	if (row && lengths) {
		return Row(row, this, lengths, throw_exceptions());
	}
	else {
		return Row();
	}
}


MYSQL_ROW
ResUse::fetch_raw_row() const
{
	return driver_->fetch_row(result_.raw());
}


int
ResUse::field_num(const std::string& i) const
{
	size_t index = (*names_)[i];
	if ((index >= names_->size()) && throw_exceptions()) {
		throw BadFieldName(i.c_str());
	}
	
	return int(index);
}


int
ResUse::num_fields() const
{
	return driver_->num_fields(result_.raw());
}


ulonglong
Result::num_rows() const
{
	return driver_ ? driver_->num_rows(result_.raw()) : 0;
}


ResUse&
ResUse::operator =(const ResUse& other)
{
	if (this != &other) {
		copy(other);
	}
	return *this;
}

} // end namespace mysqlpp

