/// \file sql_string.h
/// \brief Declares an \c std::string derivative that adds some things
/// needed within the library.
///
/// This class adds some flags needed by other parts of MySQL++, and it
/// adds conversion functions from any primitive type.  This helps in
/// inserting these primitive types into the database, because we need
/// everything in string form to build SQL queries.

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

#ifndef MYSQLPP_SQL_STRING_H
#define MYSQLPP_SQL_STRING_H

#include "defs.h"

#include <stdio.h>
#include <string>

namespace mysqlpp {

/// \brief A specialized \c std::string that will convert from any
/// valid MySQL type.

class SQLString : public std::string {
public:
	bool is_string;
	bool dont_escape;
	bool processed;

	SQLString();
	SQLString(const std::string& str);
	SQLString(const char* str);
	SQLString(char i);
	SQLString(unsigned char i);
	SQLString(short int i);
	SQLString(unsigned short int i);
	SQLString(int i);
	SQLString(unsigned int i);
	SQLString(longlong i);
	SQLString(ulonglong i);
	SQLString(float i);
	SQLString(double i);

	SQLString& operator =(const char* str) {
		std::string::operator =(str);
		return *this;
	}

	SQLString& operator =(const std::string& str) {
		std::string::operator =(str);
		return *this;
	}
};

} // end namespace mysqlpp

#endif

