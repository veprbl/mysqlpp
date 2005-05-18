/// \file const_string.h
/// \brief Declares a wrapper for <tt>const char*</tt> which behaves
/// in a way more useful to MySQL++.

/***********************************************************************
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

#ifndef MYSQLPP_CONST_STRING_H
#define MYSQLPP_CONST_STRING_H

#include "defs.h"

#include <stdexcept>
#include <string>
#include <iostream>

namespace mysqlpp {

/// \brief Wrapper for <tt>const char*</tt> to make it behave in a
/// way more useful to MySQL++.
///
/// This class implements a small subset of the standard string class.
///
/// Objects are created from an existing <tt>const char*</tt> variable
/// by copying the pointer only.  Therefore, the object pointed to by
/// that pointer needs to exist for at least as long as the const_string
/// object that wraps it.
class const_string
{
private:
	const char* str_data;

public:
	typedef const char value_type;
	typedef unsigned int size_type;
	typedef int difference_type;

	typedef const char& const_reference;
	typedef const_reference reference;
	typedef const char* const_pointer;
	typedef const_pointer pointer;

	typedef const char* const_iterator;

	/// \brief Same as const_iterator because the data cannot be
	/// changed.
	typedef const_iterator iterator;

	const_string() :
	str_data("")
	{
	}
	
	const_string(const char* str) :
	str_data(str)
	{
	}
	
	const_string& operator=(const char* str)
	{
		str_data = str;
		return *this;
	}

	size_type size() const {
		register int i = 0;
		while (str_data[i])
			 i++;
		 return i;
	}
	
	const_iterator begin() const
	{
		return str_data;
	}
	
	const_iterator end() const
	{
		return str_data + size();
	}
	
	size_type length() const
	{
		return size();
	}
	
	size_type max_size() const
	{
		return size();
	}
	
	const_reference operator[] (size_type pos) const
	{
		return str_data[pos];
	}
	
	const_reference at(size_type pos) const
	{
		if (pos >= size())
			throw std::out_of_range("");
		else
			return str_data[pos];
	}
	
	const char* c_str() const
	{
		return str_data;
	}
	
	const char* data() const
	{
		return str_data;
	}
	
	int compare(const const_string& str) const
	{
		const char* str1 = str_data;
		const char* str2 = str.str_data;
		while (*str1 == *str2 && (*str1 || *str2)) {
			str1++;
			str2++;
		}
		return *str1 - *str2;
	}
};


/// \brief Inserts a const_string into a C++ stream
inline std::ostream& operator <<(std::ostream& o,
		const const_string& str)
{
	return o << str.c_str();
}

/// \brief Calls lhs.compare(), passing rhs
inline int compare(const const_string& lhs, const const_string& rhs)
{
	return lhs.compare(rhs);
}

/// \brief Returns true if lhs is the same as rhs
inline bool operator ==(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) == 0;
}

/// \brief Returns true if lhs is not the same as rhs
inline bool operator !=(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) != 0;
}

/// \brief Returns true if lhs is lexically less than rhs
inline bool operator <(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) < 0;
}

/// \brief Returns true if lhs is lexically less or equal to rhs
inline bool operator <=(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) <= 0;
}

/// \brief Returns true if lhs is lexically greater than rhs
inline bool operator >(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) > 0;
}

/// \brief Returns true if lhs is lexically greater than or equal to rhs
inline bool operator >=(const_string& lhs, const_string& rhs)
{
	return compare(lhs, rhs) >= 0;
}

} // end namespace mysqlpp

#endif
