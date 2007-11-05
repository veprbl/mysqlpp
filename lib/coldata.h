/// \file coldata.h
/// \brief Declares classes for converting string data to any of 
/// the basic C types.
///
/// Roughly speaking, this defines classes that are the inverse of
/// mysqlpp::SQLString.

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

#ifndef MYSQLPP_COLDATA_H
#define MYSQLPP_COLDATA_H

#include "common.h"

#include "convert.h"
#include "datetime.h"
#include "exceptions.h"
#include "null.h"
#include "string_util.h"
#include "type_info.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include <stdlib.h>

namespace mysqlpp {

/// \brief A std::string work-alike that can convert itself from SQL
/// text data formats to C++ data types.
///
/// This class is an intermediate form for a SQL field, normally
/// converted to a more useful native C++ type.  It is not normally
/// used directly by MySQL++ programs, but instead is the return type
/// from several Row class members.
///
/// ColData's implicit conversion operators let you can use these
/// objects naturally:
///
/// \code ColData("12.86") + 2.0 \endcode
///
/// That will give you 14.86 (approximately) as you expect, but be
/// careful not to get tripped up by C++'s type conversion rules.  If
/// you had said this instead:
/// 
/// \code ColData("12.86") + 2 \endcode
/// 
/// the result would be 14 because 2 is an integer, and C++'s type
/// conversion rules put the ColData object in an integer context.
///
/// You can disable the operator overloads that allow these things by
/// defining MYSQLPP_NO_BINARY_OPERS.
///
/// This class also has some basic information about the type of data
/// stored in it, to allow it to do the conversions more intelligently
/// than a trivial implementation would allow.

class MYSQLPP_EXPORT ColData
{
public:
	/// \brief Type of the data stored in this object, when it is not
	/// equal to SQL null.
	typedef const char value_type;

	/// \brief Type of "size" integers
	typedef unsigned int size_type;

	/// \brief Type of iterators
	typedef const char* const_iterator;

	/// \brief Same as const_iterator because the data cannot be
	/// changed.
	typedef const_iterator iterator;

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.
	typedef int difference_type;
	typedef const char* const_pointer;
	typedef const_pointer pointer;
#endif // !defined(DOXYGEN_IGNORE)

	/// \brief Default constructor
	///
	/// An object constructed this way is essentially useless, but
	/// sometimes you just need to construct a default object.
	ColData() :
	buffer_(0)
	{
	}

	/// \brief Copy ctor
	///
	/// \param cd the other ColData object
	///
	/// This ctor only copies the pointer to the other ColData's data
	/// buffer and increments its reference counter.  If you need a
	/// deep copy, use one of the ctors that takes a string.
	ColData(const ColData& cd) :
	buffer_(cd.buffer_)
	{
		++buffer_->refs_;
	}

	/// \brief Full constructor.
	///
	/// \param str the string this object represents, or 0 for SQL null
	/// \param len the length of the string; embedded nulls are legal
	/// \param type MySQL type information for data within str
	/// \param is_null string represents a SQL null, not literal data
	///
	/// The resulting object will contain a copy of the string buffer.
	/// The buffer will actually be 1 byte longer than the value given
	/// for \c len, to hold a null terminator for safety.  We do this
	/// because this ctor may be used for things other than
	/// null-terminated C strings.  (e.g. BLOB data)
	explicit ColData(const char* str, size_type len,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false) :
	buffer_(new Buffer(str, len, type, is_null))
	{
	}

	/// \brief C++ string version of full ctor
	///
	/// \param str the string this object represents, or 0 for SQL null
	/// \param type MySQL type information for data within str
	/// \param is_null string represents a SQL null, not literal data
	///
	/// The resulting object will contain a copy of the string buffer.
	explicit ColData(const std::string& str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false) :
	buffer_(new Buffer(str.data(), str.length(), type, is_null))
	{
	}

	/// \brief Null-terminated C string version of full ctor
	///
	/// \param str the string this object represents, or 0 for SQL null
	/// \param type MySQL type information for data within str
	/// \param is_null string represents a SQL null, not literal data
	///
	/// The resulting object will contain a copy of the string buffer.
	explicit ColData(const char* str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false) :
	buffer_(new Buffer(str, strlen(str), type, is_null))
	{
	}

	/// \brief Destroy string
	~ColData();

	/// \brief Assign raw data to this object
	///
	/// This parallels the ctor with the same parameters, for when you
	/// must do a 2-step create, or when you want to reassign the data 
	/// without creating a ColData temporary to get around the fact
	/// that operator=() can only take one parameter.
	void assign(const char* str, size_type len,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false)
	{
		dec_ref_count();
		buffer_ = new Buffer(str, len, type, is_null);
	}

	/// \brief Assign a C++ string to this object
	///
	/// This parallels the ctor with the same parameters, for when you
	/// must do a 2-step create, or when you want to reassign the data 
	/// without creating a ColData temporary to get around the fact
	/// that operator=() can only take one parameter.
	void assign(const std::string& str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false)
	{
		dec_ref_count();
		buffer_ = new Buffer(str.data(), str.length(), type, is_null);
	}

	/// \brief Assign a C string to this object
	///
	/// This parallels the ctor with the same parameters, for when you
	/// must do a 2-step create, or when you want to reassign the data 
	/// without creating a ColData temporary to get around the fact
	/// that operator=() can only take one parameter.
	void assign(const char* str,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false)
	{
		dec_ref_count();
		buffer_ = new Buffer(str, strlen(str), type, is_null);
	}

	/// \brief Return a character within the string.
	///
	/// Unlike \c operator[](), this function throws an 
	/// \c std::out_of_range exception if the index isn't within range.
	char at(size_type pos) const;

	/// \brief Return iterator pointing to the first character of
	/// the string
	const_iterator begin() const { return data(); }

	/// \brief Return a const pointer to the string data.
	const char* c_str() const { return data(); }
	
	/// \brief Template for converting the column data to most any
	/// integral data type.
	template <class Type> Type conv(Type dummy) const;

	/// \brief Overload of conv() for types wrapped with Null<>
	///
	/// If the ColData object was initialized with some string we
	/// recognize as a SQL null, we just return a copy of the global
	/// 'null' object converted to the requested type.  Otherwise, we
	/// return the ColData's value wrapped in the Null<> template.
	template <class T, class B>
	Null<T, B> conv(Null<T, B> dummy) const
	{
		if (is_null()) {
			return Null<T, B>(null);
		}
		else {
			return Null<T, B>(conv(T()));
		}
	}

	/// \brief Lexically compare this string to another.
	///
	/// \param other string to compare against this one
	///
	/// \retval <0 if str1 is lexically "less than" str2
	/// \retval 0 if str1 is equal to str2
	/// \retval >0 if str1 is lexically "greater than" str2
	int compare(const ColData& other) const;

	/// \brief Raw access to the underlying buffer, with no C string
	/// interpretation.
	const char* data() const;
	
	/// \brief Return iterator pointing to one past the last character
	/// of the string.
	const_iterator end() const;

	/// \brief Returns true if data of this type should be escaped, false
	/// otherwise.
	bool escape_q() const;

	/// \brief Returns true if this object is a SQL null.
	bool is_null() const;

	/// \brief Set a flag indicating that this object is a SQL null.
	void it_is_null();

	/// \brief Return number of characters in the string
	size_type length() const;
	
	/// \brief Return the maximum number of characters in the string.
	///
	/// Because this is a \c const string, this is just an alias for
	/// size(); its size is always equal to the amount of data currently
	/// stored.
	size_type max_size() const { return size(); }

	/// \brief Returns true if data of this type should be quoted, false
	/// otherwise.
	bool quote_q() const;

	/// \brief Return number of characters in string
	size_type size() const { return length(); }
	
	/// \brief Copies this object's data into a C++ string.
	///
	/// If you know the data doesn't contain null characters (i.e. it's
	/// a typical string, not BLOB data), it's more efficient to just
	/// assign this object to anything taking \c const \c char*.  (Or
	/// equivalently, call the \c data() method.)  This copies a pointer
	/// to a buffer instead of copying the buffer's contents.
	void to_string(std::string& s) const;

	/// \brief Get this object's current MySQL type.
	mysql_type_info type() const { return buffer_->type(); }

	/// \brief Assignment operator, from C++ string
	ColData& operator =(const std::string& rhs)
	{
		dec_ref_count();

		buffer_ = new Buffer(rhs.data(), rhs.length(),
				mysql_type_info::string_type, false);

		return *this;
	}

	/// \brief Assignment operator, from C string
	///
	/// This creates a copy of the entire string, not just a copy of
	/// the pointer.
	ColData& operator =(const char* str)
	{
		dec_ref_count();

		buffer_ = new Buffer(str, strlen(str),
				mysql_type_info::string_type, false);

		return *this;
	}

	/// \brief Assignment operator, from other ColData
	///
	/// This only copies the pointer to the other ColData's data
	/// buffer and increments its reference counter.  If you need a
	/// deep copy, assign a string to this object instead.
	ColData& operator =(const ColData& cd)
	{
		dec_ref_count();

		buffer_ = cd.buffer_;
		++buffer_->refs_;

		return *this;
	}

	/// \brief Return a character within the string.
	///
	/// Unlike at(), this access method doesn't check the index for
	/// sanity.
	char operator [](size_type pos) const;

	/// \brief Returns a const char pointer to the object's raw data
	operator cchar*() const { return data(); }
	
	/// \brief Converts this object's string data to a signed char
	operator signed char() const
			{ return conv(static_cast<signed char>(0)); }
	
	/// \brief Converts this object's string data to an unsigned char
	operator unsigned char() const
			{ return conv(static_cast<unsigned char>(0)); }
	
	/// \brief Converts this object's string data to an int
	operator int() const
			{ return conv(static_cast<int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned int
	operator unsigned int() const
			{ return conv(static_cast<unsigned int>(0)); }
	
	/// \brief Converts this object's string data to a short int
	operator short int() const
			{ return conv(static_cast<short int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned short
	/// int
	operator unsigned short int() const
			{ return conv(static_cast<unsigned short int>(0)); }
	
	/// \brief Converts this object's string data to a long int
	operator long int() const
			{ return conv(static_cast<long int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned long
	/// int
	operator unsigned long int() const
			{ return conv(static_cast<unsigned long int>(0)); }
	
#if !defined(NO_LONG_LONGS)
	/// \brief Converts this object's string data to the platform-
	/// specific 'longlong' type, usually a 64-bit integer.
	operator longlong() const
			{ return conv(static_cast<longlong>(0)); }
	
	/// \brief Converts this object's string data to the platform-
	/// specific 'ulonglong' type, usually a 64-bit unsigned integer.
	operator ulonglong() const
			{ return conv(static_cast<ulonglong>(0)); }
#endif
	
	/// \brief Converts this object's string data to a float
	operator float() const
			{ return conv(static_cast<float>(0)); }
	
	/// \brief Converts this object's string data to a double
	operator double() const
			{ return conv(static_cast<double>(0)); }
	
	/// \brief Converts this object's string data to a bool
	operator bool() const { return conv(0); }

	/// \brief Converts this object's string data to a mysqlpp::Date
	operator Date() const { return Date(*this); }

	/// \brief Converts this object's string data to a mysqlpp::DateTime
	operator DateTime() const { return DateTime(*this); }

	/// \brief Converts this object's string data to a mysqlpp::Time
	operator Time() const { return Time(*this); }

	/// \brief Converts the ColData to a nullable data type
	///
	/// This is just an implicit version of conv(Null<T, B>)
	template <class T, class B>
	operator Null<T, B>() const { return conv(Null<T, B>()); }

private:
	/// \brief Decrement the buffer's reference count
	///
	/// Called by dtor and by operator=()s before replacing the buffer's
	/// contents.  If ref count falls to 0, deallocates the buffer.
	void dec_ref_count()
	{
		if (buffer_ && (--buffer_->refs_ == 0)) {
			delete buffer_;
		}
	}
	
	/// \brief Holds a ColData object's internal reference-counted
	/// string buffer.
	class Buffer {
	public:
		/// \brief Standard constructor
		///
		/// Copies the string into a new buffer one byte longer than
		/// the length value given, using that to hold a C string null
		/// terminator, just for safety.  The length value we keep does
		/// not include this extra byte, allowing this same mechanism
		/// to work for both C strings and binary data.
		Buffer(const char* data, size_type length, mysql_type_info type,
				bool is_null);

		/// \brief Destructor
		~Buffer();

		/// \brief Return pointer to raw data buffer
		const char* data() const { return data_; }

		/// \brief Return number of bytes in data buffer
		///
		/// Count does not include the trailing null we tack on to our
		/// copy of the buffer for ease of use in C string contexts.
		/// We do this because we can be holding binary data just as
		/// easily as a C string.
		size_type length() const { return length_; }

		/// \brief Return the SQL type of the data held in the buffer
		const mysql_type_info& type() const { return type_; }

		/// \brief Return true if buffer's contents represent a SQL
		/// null.
		///
		/// The buffer's actual content will probably be "NULL" or
		/// something like it, but in the SQL data type system, a SQL
		/// null is distinct from a plain string with value "NULL".
		bool is_null() const { return is_null_; }

	private:
		const char* data_;		///< pointer to the raw data buffer
		size_type length_;		///< bytes in buffer, without trailing null
		mysql_type_info type_;	///< SQL type of data in the buffer
		bool is_null_;			///< if true, string represents a SQL null
		unsigned int refs_;		///< reference count for this object

		friend class ColData;	// allow our parent to modify us
	} *buffer_;
};



#if !defined(MYSQLPP_NO_BINARY_OPERS) && !defined(DOXYGEN_IGNORE)
// Ignore this section is MYSQLPP_NO_BINARY_OPERS is defined, or if this
// section is being parsed by Doxygen.  In the latter case, it's ignored
// because Doxygen doesn't understand it correctly, and we can't be
// bothered to explain it to Doxygen.

#define oprsw(opr, other, conv) \
	inline other operator opr (ColData x, other y) \
			{ return static_cast<conv>(x) opr y; } \
	inline other operator opr (other x, ColData y) \
			{ return x opr static_cast<conv>(y); }

#define operator_binary(other, conv) \
  oprsw(+, other, conv) \
  oprsw(-, other, conv) \
  oprsw(*, other, conv) \
  oprsw(/, other, conv)

#define operator_binary_int(other, conv) \
  operator_binary(other, conv) \
  oprsw(%, other, conv) \
  oprsw(&, other, conv) \
  oprsw(^, other, conv) \
  oprsw(|, other, conv) \
  oprsw(<<, other, conv) \
  oprsw(>>, other, conv)

operator_binary(float, double)
operator_binary(double, double)

operator_binary_int(char, long int)
operator_binary_int(int, long int)
operator_binary_int(short int, long int)
operator_binary_int(long int, long int)

operator_binary_int(unsigned char, unsigned long int)
operator_binary_int(unsigned int, unsigned long int)
operator_binary_int(unsigned short int, unsigned long int)
operator_binary_int(unsigned long int, unsigned long int)

#if !defined(NO_LONG_LONGS)
operator_binary_int(longlong, longlong)
operator_binary_int(ulonglong, ulonglong)
#endif
#endif // MYSQLPP_NO_BINARY_OPERS

// The generic conv() implementation for integral types.
template <class Type>
Type ColData::conv(Type /* dummy */) const
{
	std::string strbuf(data(), length());
	strip_all_blanks(strbuf);
	std::string::size_type len = strbuf.size();
	const char* str = strbuf.data();
	const char* end = str;
	Type num = internal_string_to_int_proxy<Type>(str, end);

	if (*end == '.') {
		++end;
		for (; *end == '0'; ++end) ;
	}
	
	if (*end != '\0' && end != 0) {
		throw BadConversion(typeid(Type).name(), data(),
				end - str, len);
	}

	return num;
}


/// \brief Specialization of ColData::conv<Type>() for ColData
///
/// Yes, I hear you crying, "WTF?"  Why does ColData need to be able to
/// convert itself to ColData?  SSQLSes with BLOB columns, that's why.
///
/// SSQLSes populate their data members from the raw field data by
/// calling row[field].conv().  The raw field data is stored in a
/// ColData, and the MySQL++ native BLOB type is ColData.  Since we're
/// dealing with generated code, we need this specialization which hand-
/// written code wouldn't need.  Prove the truth of this to yourself by
/// removing this and counting how many pieces examples/cgi_jpeg.cpp
/// breaks into.
template <> ColData ColData::conv(ColData dummy) const;

/// \brief Specialization of ColData::conv<Type>() for C++ strings
template <> std::string ColData::conv(std::string dummy) const;

/// \brief Specialization of ColData::conv<Type>() for mysqlpp::Date
///
/// This is necessary because as of MySQL++ v3, Date no longer has an
/// implicit conversion ctor from ColData, and SSQLS uses conv() instead
/// of the C++ type conversion system anyway.
template <> Date ColData::conv(Date dummy) const;

/// \brief Specialization of ColData::conv<Type>() for mysqlpp::DateTime
///
/// This is necessary because as of MySQL++ v3, DateTime no longer has
/// an implicit conversion ctor from ColData, and SSQLS uses conv()
/// instead of the C++ type conversion system anyway.
template <> DateTime ColData::conv(DateTime dummy) const;

/// \brief Specialization of ColData::conv<Type>() for mysqlpp::Time
///
/// This is necessary because as of MySQL++ v3, Time no longer has an
/// implicit conversion ctor from ColData, and SSQLS uses conv() instead
/// of the C++ type conversion system anyway.
template <> Time ColData::conv(Time dummy) const;

} // end namespace mysqlpp

#endif
