/***********************************************************************
 row.cpp - Implements the Row class.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

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

#include "row.h"
#include "result.h"
#include "exceptions.h"

namespace mysqlpp {

Row::size_type Row::size() const
{
	return res_->num_fields();
}

const ColData Row::at(size_type i) const
{
	if (initialized_) {
		return ColData(data_.at(i).c_str(), res_->types(i), is_nulls_[i]);
	}
	else {
		if (throw_exceptions())
			throw std::out_of_range("Row not initialized");
		else
			return ColData();
	}
}

const ColData Row::operator [](const char* field) const
{
	int si = res_->field_num(std::string(field));
	if (si < size()) {
		return at(si);
	}
	else {
		throw BadFieldName(field);
	}
}

} // end namespace mysqlpp

