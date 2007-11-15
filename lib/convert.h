/// \file convert.h
/// \brief Declares various mechanisms MySQL++ uses internally to
/// convert data from one type to another.
///
/// This is the mechanism behind much of the mysqlpp::String::conv()
/// stuff.  This stuff should not be used by end-user code.

/***********************************************************************
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

#ifndef MYSQLPP_CONVERT_H
#define MYSQLPP_CONVERT_H

#include "common.h"

#include <stdlib.h>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

template <class Type> class internal_string_to_int_proxy;

#define internal_convert_string_to_int(TYPE, FUNC) \
  template <> \
  class internal_string_to_int_proxy<TYPE> {\
  public:\
    internal_string_to_int_proxy(const char* str, const char *& end) { \
      num_ = FUNC(str, const_cast<char **>(&end));}\
    operator TYPE () {return num_;}\
  private:\
    TYPE num_;\
  };\

#if defined(_MSC_VER)
#	pragma warning(disable: 4244)
#endif

	internal_convert_string_to_int(float, strtod)
	internal_convert_string_to_int(double, strtod)

#if defined(_MSC_VER)
#	pragma warning(default: 4244)
#endif

#undef internal_convert_string_to_int
#define internal_convert_string_to_int(TYPE, FUNC) \
  template <> \
  class internal_string_to_int_proxy<TYPE> {\
  public:\
    internal_string_to_int_proxy(const char* str, const char *& end) { \
      num_ = FUNC(str, const_cast<char **>(&end),10);}\
    operator TYPE () {return num_;}\
  private:\
    TYPE num_;\
  };\

#if defined(_MSC_VER)
#	pragma warning(disable: 4244)
#endif

	internal_convert_string_to_int(char, strtol)
	internal_convert_string_to_int(signed char, strtol)
	internal_convert_string_to_int(int, strtol)
	internal_convert_string_to_int(short int, strtol)
	internal_convert_string_to_int(long int, strtol)

	internal_convert_string_to_int(unsigned char, strtoul)
	internal_convert_string_to_int(unsigned int, strtoul)
	internal_convert_string_to_int(unsigned short int, strtoul)
	internal_convert_string_to_int(unsigned long int, strtoul)

#if defined(_MSC_VER)
#	pragma warning(default: 4244)
#endif

#if !defined(NO_LONG_LONGS)
#if defined(_MSC_VER)
// Handle 64-bit ints the VC++ way
internal_convert_string_to_int(longlong, _strtoi64)
internal_convert_string_to_int(ulonglong, _strtoui64)
#else
// No better idea, so assume the C99 way.  If your compiler doesn't
// support this, please provide a patch to extend this ifdef, or define
// NO_LONG_LONGS.
internal_convert_string_to_int(longlong, strtoll)
internal_convert_string_to_int(ulonglong, strtoull)
#endif
#endif // !defined(NO_LONG_LONGS)

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif

