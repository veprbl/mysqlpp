/// \file field_types.h
/// \brief Declares a class to hold a list of SQL field type info.

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

#ifndef MYSQLPP_FIELD_TYPES_H
#define MYSQLPP_FIELD_TYPES_H

#include "type_info.h"

#include <vector>

namespace mysqlpp {

class ResUse;

/// \brief A vector of SQL field types.
class FieldTypes : public std::vector<mysql_type_info> {
private:
  void init (const ResUse *res);
public:
  FieldTypes () {}
  FieldTypes (const ResUse *res) {init(res);}
  FieldTypes (int i) : std::vector<mysql_type_info>(i) {}

  /// \brief Creates a new list based on the info in res
  FieldTypes& operator =(const ResUse *res) {init(res); return *this;}

  /// \brief Creates a new list with a given number of fields.
  ///
  /// Initializes the list from the data returned by MySQL C API
  /// function \c mysql_type_info().
  ///
  /// \param i size of field list to create
  FieldTypes& operator =(int i)
  {
  	insert(begin(), i, mysql_type_info());
	return *this;
  }
  
  /// \brief Returns a field type within the list given its index.
  mysql_type_info& operator [](int i)
  {
  	return std::vector<mysql_type_info>::operator[](i);
  }
  const mysql_type_info& operator [](int i) const 
  {
  	return std::vector<mysql_type_info>::operator[](i);
  }
};

} // end namespace mysqlpp

#endif

