/// \file sql_types.h
/// \brief Declares the closest C++ equivalent of each MySQL column type

/***********************************************************************
 Copyright (c) 2006-2008 by Educational Technology Resources, Inc.
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

#if !defined(MYSQLPP_SQL_TYPES_H_MAIN)
#define MYSQLPP_SQL_TYPES_H_MAIN

#include "tiny_int.h"

#include <string>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

// Nearest C++ equivalents of MySQL data types.  These are only the "NOT
// NULL" variants.  Wrap these types in MySQL++'s Null<> template to get
// NULL-able types.
typedef tiny_int<signed char>	sql_tinyint;
typedef tiny_int<unsigned char>	sql_tinyint_unsigned;
typedef short					sql_smallint;
typedef unsigned short			sql_smallint_unsigned;
typedef int						sql_int;
typedef unsigned int			sql_int_unsigned;
typedef int						sql_mediumint;
typedef unsigned int			sql_mediumint_unsigned;
typedef longlong				sql_bigint;
typedef ulonglong				sql_bigint_unsigned;

typedef float					sql_float;
typedef double					sql_double;
typedef double					sql_decimal;

typedef std::string				sql_enum;

typedef std::string				sql_char;
typedef std::string				sql_varchar;
typedef std::string				sql_tinytext;
typedef std::string				sql_text;
typedef std::string				sql_mediumtext;
typedef std::string				sql_longtext;

// Aliases to match the rules MySQL uses in translating data types
// from other database servers into its own type system.  From:
// http://dev.mysql.com/doc/refman/5.0/en/other-vendor-data-types.html
typedef sql_tinyint				sql_bool;
typedef sql_tinyint				sql_boolean;
typedef sql_varchar				sql_character_varying;
typedef sql_decimal				sql_fixed;
typedef sql_float				sql_float4;
typedef sql_double				sql_float8;
typedef sql_tinyint				sql_int1;
typedef sql_smallint			sql_int2;
typedef sql_mediumint			sql_int3;
typedef sql_int					sql_int4;
typedef sql_bigint				sql_int8;
typedef sql_mediumtext			sql_long_varchar;
typedef sql_mediumtext			sql_long;
typedef sql_mediumint			sql_middleint;
typedef sql_decimal				sql_numeric;
#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SQL_TYPES_H_MAIN)


// The following sections are treated separately to avoid making the
// #include tree too dense: if mystring.h (for example) is not yet
// #included, no sense pulling it in to define all the typedefs based
// on String.  The separate #include guards for each section allow
// this file to be #included as many times as necessary to build up the
// full typedef set.  This trickery is necessary because sql_types.h
// is needed in a few places within MySQL++, but we can't (and don't)
// depend on having the full set of typedefs.  mysql++.h #includes this
// at a late stage, ensuring that end-user code does see the full set.
#if defined(MYSQLPP_MYSTRING_H) && !defined(MYSQLPP_SQL_TYPES_H_MYSTRING) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_MYSTRING
	namespace mysqlpp {
		typedef String			sql_blob;
		typedef String			sql_tinyblob;
		typedef String			sql_mediumblob;
		typedef String			sql_longblob;
		typedef sql_mediumblob	sql_long_varbinary;
	} // end namespace mysqlpp
#endif


#if defined(MYSQLPP_DATETIME_H) && !defined(MYSQLPP_SQL_TYPES_H_DATETIME) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_DATETIME
	namespace mysqlpp {
		typedef Date			sql_date;
		typedef Time			sql_time;
		typedef DateTime		sql_timestamp;
		typedef DateTime		sql_datetime;
	} // end namespace mysqlpp
#endif


#if defined(MYSQLPP_MYSET_H) && !defined(MYSQLPP_SQL_TYPES_H_SET) && !defined(DOXYGEN_IGNORE)
#	define MYSQLPP_SQL_TYPES_H_SET
	namespace mysqlpp {
		typedef Set<>				sql_set;
	} // end namespace mysqlpp
#endif

