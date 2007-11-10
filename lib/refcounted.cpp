/***********************************************************************
 refcounted.cpp - Implements the RefCountedBuffer class.

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

#include "refcounted.h"

namespace mysqlpp {


void
RefCountedBuffer::init(const char* pd, size_type length,
		mysql_type_info type, bool is_null)
{
	type_ = type;
	is_null_ = is_null;
	refs_ = 1;

	if (pd) {
		// The casts for the data member are because the C type system
		// can't distinguish initialization from modification.  We don't
		// care to enforce constness on this buffer until after this
		// function returns.  The cast for pd is just because memcpy()
		// doesn't declare its second parameter const for historical
		// reasons, not because it actually does modify it.
		data_ = new char[length + 1];
		length_ = length;
		memcpy(const_cast<char*>(data_), const_cast<char*>(pd), length_);
		const_cast<char*>(data_)[length_] = '\0';
	}
	else {
		data_ = 0;
		length_ = 0;
	}
}

} // end namespace mysqlpp

