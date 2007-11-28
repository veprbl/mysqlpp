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

namespace mysqlpp {

ResUse::ResUse(MYSQL_RES* result, bool te) :
OptionalExceptions(te),
result_(0),
initialized_(false),
types_(0),
fields_(this)
{
	if (result) {
		result_ = result;
		names_ = new FieldNames(this);
		types_ = new FieldTypes(this);
		initialized_ = true;
	}
}


ResUse::~ResUse()
{
	purge();
}


void
ResUse::copy(const ResUse& other)
{
	if (initialized_) {
		purge();
	}

	set_exceptions(other.throw_exceptions());

	if (other.result_) {
		result_ = other.result_;
		fields_ = Fields(this);
		names_ = other.names_;
		
		if (other.types_) {
			types_ = new FieldTypes(*other.types_);
		}
		else {
			types_ = 0;
		}

		initialized_ = true;
	}
	else {
		result_ = 0;
		types_ = 0;
		names_ = 0;
		initialized_ = other.initialized_;
	}
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


void
ResUse::purge()
{
	if (result_) {
		mysql_free_result(result_);
		result_ = 0;
	}

	names_ = 0;

	delete types_;
	types_ = 0;
}


ResUse&
ResUse::operator =(const ResUse& other)
{
	if (this == &other) {
		return *this;
	}
	copy(other);
	other.result_ = 0;
	return *this;
}

} // end namespace mysqlpp

