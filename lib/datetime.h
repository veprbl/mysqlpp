/// \file datetime.h
/// \brief Declares classes to add MySQL-compatible date and time
/// types to C++'s type system.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2008 by Educational Technology Resources, Inc.  Others may
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

#if !defined(MYSQLPP_DATETIME_H)
#define MYSQLPP_DATETIME_H

#include "common.h"

#include <string>
#include <iostream>

namespace mysqlpp {

/// \brief Base class template for MySQL++ date and time classes.
///
/// This template primarily defines the comparison operators, which are
/// all implemented in terms of compare(). Each subclass implements that
/// as a protected method, because these operators are the only
/// supported comparison method.
///
/// This template also defines interfaces for converting the object to
/// a string form, which a subclass must define.
template <class T>
struct MYSQLPP_EXPORT DTbase
{
	/// \brief Destroy object
	virtual ~DTbase() { }

	/// \brief Return a copy of the item in C++ string form
	operator std::string() const;

	/// \brief Compare this object to another of the same type
	///
	/// Returns < 0 if this object is "before" the other, 0 of they are
	/// equal, and > 0 if this object is "after" the other.
	virtual int compare(const T& other) const = 0;

	/// \brief Returns true if "other" is equal to this object
	bool operator ==(const T& other) const
	{
		return !compare(other);
	}

	/// \brief Returns true if "other" is not equal to this object
	bool operator !=(const T& other) const
	{
		return compare(other);
	}

	/// \brief Returns true if "other" is less than this object
	bool operator <(const T& other) const
	{
		return compare(other) < 0;
	}

	/// \brief Returns true if "other" is less than or equal to this object
	bool operator <=(const T& other) const
	{
		return compare(other) <= 0;
	}

	/// \brief Returns true if "other" is greater than this object
	bool operator >(const T& other) const
	{
		return compare(other) > 0;
	}

	/// \brief Returns true if "other" is greater than or equal to this object
	bool operator >=(const T& other) const
	{
		return compare(other) >= 0;
	}
};


/// \brief C++ form of MySQL's DATETIME type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL DATETIME strings.
///
/// If you use the default constructor, it's sensible to use the object
/// in a SQL query as of MySQL++ v3.0.  When converting the object to a
/// string form for assembling the query, we convert to the SQL function
/// "NOW()", allowing the database server to set the value.  In older
/// versions of MySQL++, you'd get a "zero time" instead.

struct MYSQLPP_EXPORT DateTime : public DTbase<DateTime>
{
	unsigned short year;	///< the year, as a simple integer
	unsigned char month;	///< the month, 1-12
	unsigned char day;		///< the day, 1-31
	unsigned char hour;		///< the hour, 0-23
	unsigned char minute;	///< the minute, 0-59
	unsigned char second;	///< the second, 0-59
	bool now;				///< true if not given explicit value

	/// \brief Default constructor
	DateTime() :
	DTbase<DateTime>(),
	year(0),
	month(0),
	day(0),
	hour(0),
	minute(0),
	second(0),
	now(true)
	{
	}

	/// \brief Initialize object from discrete y/m/d h:m:s values.
	///
	/// \param y year
	/// \param mon month
	/// \param d day of month
	/// \param h hour
	/// \param min minute
	/// \param s second
	DateTime(unsigned short y, unsigned char mon, unsigned char d,
			unsigned char h, unsigned char min, unsigned char s) :
	DTbase<DateTime>(),
	year(y),
	month(mon),
	day(d),
	hour(h),
	minute(min),
	second(s),
	now(false)
	{
	}
	
	/// \brief Initialize object as a copy of another Date
	DateTime(const DateTime& other) :
	DTbase<DateTime>(),
	year(other.year),
	month(other.month),
	day(other.day),
	hour(other.hour),
	minute(other.minute),
	second(other.second),
	now(false)
	{
	}

	/// \brief Initialize object from a C string containing a MySQL
	/// date-and-time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	explicit DateTime(cchar* str) { convert(str); }
	
	/// \brief Initialize object from a C++ type of string containing
	/// a MySQL date-and-time string
	///
	/// \sa DateTime(cchar*)
	template <class Str>
	explicit DateTime(const Str& str)
	{
		convert(str.c_str());
	}

	/// \brief Initialize object from a time_t
	explicit DateTime(time_t t);

	/// \brief Compare this datetime to another.
	///
	/// Returns < 0 if this datetime is before the other, 0 of they are
	/// equal, and > 0 if this datetime is after the other.
	///
	/// This method is protected because it is merely the engine used
	/// by the various operators in DTbase.
	int compare(const DateTime& other) const;

	/// \brief Parse a MySQL date and time string into this object.
	cchar* convert(cchar*);

	/// \brief Returns true if 'now' member is true and all other
	/// members are zero.
	///
	/// Can't just test 'now' because you can create the object with
	/// the default ctor (giving a "now" value) and then assign values
	/// to its data members later, which should make it "not now".  We
	/// could do without this by hiding all the data members behind
	/// accessors which can reset the "now" flag when setting other
	/// values, but it's too simple a class to bother.
	bool is_now() const;

	/// Convert to time_t
	operator time_t() const;
};


/// \brief Inserts a DateTime object into a C++ stream in a
/// MySQL-compatible format.
///
/// The date and time are inserted into the stream, in that order,
/// with a space between them.
///
/// \param os stream to insert date and time into
/// \param dt date/time object to insert into stream
MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& os,
		const DateTime& dt);


/// \brief C++ form of MySQL's DATE type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL DATE strings.
struct MYSQLPP_EXPORT Date : public DTbase<Date>
{
	unsigned short year;	///< the year, as a simple integer
	unsigned char month;	///< the month, 1-12
	unsigned char day;		///< the day, 1-31

	/// \brief Default constructor
	Date() : year(0), month(0), day(0) { }

	/// \brief Initialize object
	Date(unsigned short y, unsigned char m, unsigned char d) :
	DTbase<Date>(),
	year(y),
	month(m),
	day(d)
	{
	}
	
	/// \brief Initialize object as a copy of another Date
	Date(const Date& other) :
	DTbase<Date>(),
	year(other.year),
	month(other.month),
	day(other.day)
	{
	}

	/// \brief Initialize object from date part of date/time object
	Date(const DateTime& other) :
	DTbase<Date>(),
	year(other.year),
	month(other.month),
	day(other.day)
	{
	}

	/// \brief Initialize object from a C string containing a date
	///
	/// String must be in the YYYY-MM-DD format.  It doesn't have to be
	/// zero-padded.
	explicit Date(cchar* str) { convert(str); }
	
	/// \brief Initialize object from a C++ style string containing a date
	///
	/// \sa Date(cchar*)
	template <class Str>
	explicit Date(const Str& str) { convert(str.c_str()); }

	/// \brief Compare this date to another.
	///
	/// Returns < 0 if this date is before the other, 0 of they are
	/// equal, and > 0 if this date is after the other.
	int compare(const Date& other) const;

	/// \brief Parse a MySQL date string into this object.
	cchar* convert(cchar*);
};

/// \brief Inserts a Date object into a C++ stream
///
/// The format is YYYY-MM-DD, zero-padded.
///
/// \param os stream to insert date into
/// \param d date to insert into stream
MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& os,
		const Date& d);


/// \brief C++ form of MySQL's TIME type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from MySQL TIME strings.
struct MYSQLPP_EXPORT Time : public DTbase<Time>
{
	unsigned char hour;		///< the hour, 0-23
	unsigned char minute;	///< the minute, 0-59
	unsigned char second;	///< the second, 0-59

	/// \brief Default constructor
	Time() : hour(0), minute(0), second(0) { }

	/// \brief Initialize object
	Time(unsigned char h, unsigned char m, unsigned char s) :
	hour(h),
	minute(m),
	second(s)
	{
	}

	/// \brief Initialize object as a copy of another Time
	Time(const Time& other) :
	DTbase<Time>(),
	hour(other.hour),
	minute(other.minute),
	second(other.second)
	{
	}

	/// \brief Initialize object from time part of date/time object
	Time(const DateTime& other) :
	DTbase<Time>(),
	hour(other.hour),
	minute(other.minute),
	second(other.second)
	{
	}

	/// \brief Initialize object from a C string containing a MySQL
	/// time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	explicit Time(cchar* str) { convert(str); }

	/// \brief Initialize object from a C++ type string containing a
	/// MySQL time string
	///
	/// \sa Time(cchar*)
	template <class Str>
	explicit Time(const Str& str) { convert(str.c_str()); }

	/// \brief Parse a MySQL time string into this object.
	cchar* convert(cchar*);

	/// \brief Compare this time to another.
	///
	/// Returns < 0 if this time is before the other, 0 of they are
	/// equal, and > 0 if this time is after the other.
	int compare(const Time& other) const;
};

/// \brief Inserts a Time object into a C++ stream in a MySQL-compatible
/// format.
///
/// The format is HH:MM:SS, zero-padded.
///
/// \param os stream to insert time into
/// \param t time to insert into stream
MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& os,
		const Time& t);


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_DATETIME_H)
