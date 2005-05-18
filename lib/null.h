/// \file null.h
/// \brief Declares classes that implement SQL "null" semantics within
/// C++'s type system.
///
/// This is required because C++'s own NULL type is not semantically
/// the same as SQL nulls.

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

#ifndef MYSQLPP_NULL_H
#define MYSQLPP_NULL_H

#include "exceptions.h"

#include <iostream>

namespace mysqlpp {


/// \brief The type of the global mysqlpp::null object.
///
/// This class is for internal use only.  Normal code should use
/// Null instead.
class null_type {
public:
  template <class Type> operator Type () {throw BadNullConversion();}
};

/// \brief Global 'null' instance.  Use wherever you need a SQL null.
/// (As opposed to a C language null pointer or null character.)
const null_type null = null_type();

/// \brief Used for the behavior parameter for template Null
struct NullisNull {
  static null_type null_is() {return null_type();}
  static std::ostream&  null_ostr(std::ostream& o) {o << "(NULL)"; return o;}
};

/// \brief Used for the behavior parameter for template Null
struct NullisZero {
  static int   null_is() {return 0;}
  static std::ostream& null_ostr(std::ostream &o) {o << 0; return o;}
};

/// \brief Used for the behavior parameter for template Null
struct NullisBlank {
  static const char * null_is() {return "";}
  static std::ostream& null_ostr(std::ostream &o) {o << ""; return o;}
};


/// \brief Container class for holding SQL nulls.
///
/// This template is necessary because there is nothing in the C++ type
/// system with the same semantics as SQL's null.  (No, NULL from
/// stddef.h is not the same!)  This template mechanism also
/// makes the typeid() unique for each C++-equivalent of nullable
/// SQL field types, which are listed in type_info.cpp.
template <class Type, class Behavior = NullisNull>
class Null {
public:
  Type data;
  bool is_null;
  typedef Type value_type;
public:
  Null () {}
  Null (Type x) : data(x), is_null(false) {} 

  /// \brief Gives Null the null value
  ///
  /// The global const \c null (not to be confused with C's NULL type)
  /// is of type null_type, so you can say something like:
  /// \code
  /// Null<Type> foo = null;
  /// \endcode
  Null (const null_type &n) : is_null(true) {} 

  operator Type& () { 
    if (is_null) return data = Behavior::null_is();
    else return data; } 

  Null& operator = (const Type &x) {data = x; is_null = false; return *this;}

  Null& operator = (const null_type &n) {is_null = true; return *this;}
};


/// \if INTERNAL
// Doxygen will not generate documentation for this section.

// Specialization the Null template for \c void
template <> class Null<void> {
public:
  bool is_null;
  typedef void value_type;
public:
  Null () : is_null(false) { } 
  Null (const null_type &) : is_null(true) { } 
  Null& operator = (const null_type &) { is_null = true; return *this; }
};

/// \endif
 

/// \brief Inserts null-able data into a C++ stream if it is not
/// actually null.  Otherwise, insert something appropriate for null
/// data.
template <class Type, class Behavior>
inline std::ostream& operator <<(std::ostream& o,
		const Null<Type,Behavior>& n)
{
  if (n.is_null) return Behavior::null_ostr(o);
  else return o << n.data;
}

} // end namespace mysqlpp

#endif

