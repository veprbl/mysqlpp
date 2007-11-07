/// \file stadapter.h
/// \brief Declares the SQLTypeAdapter class
///
/// This class provides implicit conversion between many C++ types and
/// SQL-formatted string representations of that data while preserving
/// some type information.  End-users of MySQL++ are not expected to
/// use this class directly.  Rather, it exists for those interfaces in
/// MySQL++ that need to accept any of several reasonable data types,
/// particularly those involved in building query strings.
///
/// There are two main uses for this.  One is in the Query class
/// interfaces for building template queries: they have to be generic
/// with respect to argument type, but because we know we want the data
/// in some kind of string form eventually, we don't need to templatize
/// it.  The interface can just accept a SQLTypeAdapter, which lets it
/// accept any reasonable data type.
///
/// The other major use for this type is the quoting and escaping logic
/// in Query's stream interface: rather than overload the << operators
/// and the manipulators for every single type we know the rules for \e a 
/// \e priori, we just specialize the manipulators for SQLTypeAdapter.
/// The conversion to SQLTypeAdapter stringizes the data, which we needed
/// anyway for stream insertion, and holds enough type information so
/// that the manipulator can decide whether to do automatic quoting
/// and/or escaping.

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

#if !defined(MYSQLPP_SQL_TYPE_ADAPTER_H)
#define MYSQLPP_SQL_TYPE_ADAPTER_H

#include "common.h"

#include "datetime.h"
#include "null.h"

#include <string>

namespace mysqlpp {

/// \brief A specialized \c std::string that will convert from any
/// valid MySQL type.

class MYSQLPP_EXPORT SQLTypeAdapter : public std::string
{
public:
	/// \brief Default constructor; empty string
	SQLTypeAdapter();

	/// \brief Create a copy of a C++ string
	SQLTypeAdapter(const std::string& str);

	/// \brief Create a copy of a null-terminated C string
	SQLTypeAdapter(const char* str);

	/// \brief Create a copy of an arbitrary block of data
	SQLTypeAdapter(const char* str, size_t len);

	/// \brief Create a string representation of a \c char treated as a
	/// small integer (not a character!)
	SQLTypeAdapter(signed char i);

	/// \brief Create a string representation of an \c unsigned \c char
	/// treated as a small unsigned integer (not a character!)
	SQLTypeAdapter(unsigned char i);

	/// \brief Create a string representation of a \c short \c int value
	SQLTypeAdapter(short int i);

	/// \brief Create a string representation of an \c unsigned
	/// \c short \c int value
	SQLTypeAdapter(unsigned short int i);

	/// \brief Create a string representation of an \c int value
	SQLTypeAdapter(int i);

	/// \brief Create a string representation of an \c unsigned \c int
	/// value
	SQLTypeAdapter(unsigned int i);

	/// \brief Create a string representation of a \c longlong value
	SQLTypeAdapter(longlong i);

	/// \brief Create a string representation of an \c unsigned
	/// \c longlong value
	SQLTypeAdapter(ulonglong i);

	/// \brief Create a string representation of a \c float value
	SQLTypeAdapter(float i);

	/// \brief Create a string representation of a \c double value
	SQLTypeAdapter(double i);

	/// \brief Create a SQL string representation of a date
	SQLTypeAdapter(const Date& d);

	/// \brief Create a SQL string representation of a date and time
	SQLTypeAdapter(const DateTime& dt);

	/// \brief Create a SQL string representation of a time
	SQLTypeAdapter(const Time& t);

	/// \brief Create object representing SQL NULL
	SQLTypeAdapter(const null_type& i);

	/// \brief Copy a C string into this object
	SQLTypeAdapter& operator =(const char* str)
	{
		std::string::operator =(str);
		is_string_ = true;
		is_processed_ = false;
		return *this;
	}

	/// \brief Copy a C++ \c string into this object
	SQLTypeAdapter& operator =(const std::string& str)
	{
		std::string::operator =(str);
		is_string_ = true;
		is_processed_ = false;
		return *this;
	}

	/// \brief Returns true if the object was initialized with a
	/// stringish type
	///
	/// By contrast, this returns false if we were initialized with,
	/// say, a floating point number.  This is used for deciding whether
	/// to do quoting and/or escaping when building query strings.
	bool is_string() const { return is_string_; }

	/// \brief Returns true if the internal 'processed' flag is set.
	///
	/// This is an implementation detail of template queries, used to
	/// prevent repeated processing of values.
	bool is_processed() const { return is_processed_; }

	/// \brief Turns on the internal 'is_processed_' flag.
	///
	/// This is an implementation detail of template queries, used to
	/// prevent repeated processing of values.
	void set_processed() { is_processed_ = true; }

#if !defined(DOXYGEN_IGNORE)
	// Parallel interface for Null<>-wrapped versions of types we
	// support above.  No need for parallel documentation.
	SQLTypeAdapter(const Null<std::string>& str);
	SQLTypeAdapter(Null<char> i);
	SQLTypeAdapter(Null<unsigned char> i);
	SQLTypeAdapter(Null<short int> i);
	SQLTypeAdapter(Null<unsigned short int> i);
	SQLTypeAdapter(Null<int> i);
	SQLTypeAdapter(Null<unsigned int> i);
	SQLTypeAdapter(Null<longlong> i);
	SQLTypeAdapter(Null<ulonglong> i);
	SQLTypeAdapter(Null<float> i);
	SQLTypeAdapter(Null<double> i);
	SQLTypeAdapter(const Null<Date>& d);
	SQLTypeAdapter(const Null<DateTime>& dt);
	SQLTypeAdapter(const Null<Time>& t);
	SQLTypeAdapter& operator =(const Null<std::string>& str)
			{ return operator =(str.data); }
#endif // !defined(DOXYGEN_IGNORE)

private:
	/// \brief If true, the object's string data is a copy of another
	/// string.  Otherwise, it's the string form of an integral type.
	bool is_string_;

	/// \brief If true, one of the MySQL++ manipulators has processed
	/// the string data.
	///
	/// "Processing" is escaping special SQL characters, and/or adding
	/// quotes.  See the documentation for manip.h for details.
	///
	/// This flag is used by the template query mechanism, to prevent a
	/// string from being re-escaped or re-quoted each time that query
	/// is reused.  The flag is reset by operator=, to force the new
	/// parameter value to be re-processed.
	bool is_processed_;
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SQL_TYPE_ADAPTER_H)
