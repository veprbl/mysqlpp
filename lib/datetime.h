/// \file datetime.h
/// \brief Declares classes to add MySQL-compatible date and time
/// types to C++'s type system.

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

#ifndef MYSQLPP_DATETIME_H
#define MYSQLPP_DATETIME_H

#include "defs.h"

#include "coldata.h"
#include "stream2string.h"
#include "tiny_int.h"

#include <string>
#include <sstream>
#include <iostream>

namespace mysqlpp {

/// \brief Base class for mysql_date and mysql_time
struct mysql_dt_base
{
	/// \if INTERNAL
	// Make Doxygen ignore this function; struct docs are sufficient
	virtual std::ostream& out_stream(std::ostream&) const = 0;
	operator std::string() const;
	/// \endif
};


/// \if INTERNAL
// Doxygen will not generate documentation for this section.

template <class T> struct DTbase
{
	virtual short int compare(const T& other) const = 0;

	bool operator ==(const T& other) const { return !compare(other); }
	bool operator !=(const T& other) const { return compare(other); }
	bool operator <(const T& other) const { return compare(other) < 0; }
	bool operator <=(const T& other) const { return compare(other) <= 0; }
	bool operator >(const T& other) const { return compare(other) > 0; }
	bool operator >=(const T& other) const { return compare(other) >= 0; }
};

/// \endif


/// \brief Base class of Date
struct mysql_date : virtual public mysql_dt_base
{
	short int year;
	tiny_int month;
	tiny_int day;

	/// \brief Insert the date into a stream
	///
	/// The date is inserted into the stream in a format that the MySQL
	/// server can accept.
	///
	/// \param os stream to insert date into
	std::ostream& out_stream(std::ostream& os) const;

	/// \brief Parse a MySQL date string into this object.
	cchar* convert(cchar*);

protected:
	short int compare(const mysql_date* other) const;
};


/// \brief Holds MySQL dates.
///
/// Objects of this class can be inserted into streams, and
/// initialized from a stream.

struct Date : public mysql_date, public DTbase<Date>
{
	Date() { };

	Date(cchar* str)
	{
		convert(str);
	}
	
	Date(const ColData& str);

	Date(const std::string& str);

	short int compare(const Date& other) const
	{
		return mysql_date::compare(&other);
	}
};


/// \brief Inserts a Date object into a C++ stream in a MySQL-compatible
/// format.

inline std::ostream& operator <<(std::ostream& s, const Date& d)
{
	return d.out_stream(s);
}


/// \brief Base class of Time

struct mysql_time : virtual public mysql_dt_base
{
	tiny_int hour;
	tiny_int minute;
	tiny_int second;

	/// \brief Insert the time into a stream
	///
	/// The time is inserted into the stream in a format that the MySQL
	/// server can accept.
	///
	/// \param os stream to insert time into
	std::ostream& out_stream(std::ostream& os) const;

	/// \brief Parse a MySQL time string into this object.
	cchar* convert(cchar*);

protected:
	short int compare(const mysql_time* other) const;
};


/// \brief Holds MySQL times.
///
/// Objects of this class can be inserted into streams, and
/// initialized from a stream.
struct Time : public mysql_time, public DTbase<Time>
{
	Time() { };

	Time(cchar* str)
	{
		convert(str);
	}

	Time(const ColData& str);

	Time(const std::string& str);

	short int compare(const Time& other) const
	{
		return mysql_time::compare(&other);
	}
};


/// \brief Inserts a Time object into a C++ stream in a MySQL-compatible
/// format.

inline std::ostream& operator <<(std::ostream& s, const Time& d)
{
	return d.out_stream(s);
}


/// \brief A combination of the Date and Time classes for holding
/// MySQL DateTimes.
///
/// Objects of this class can be inserted into streams, and
/// initialized from a stream.

struct DateTime : public mysql_date, public mysql_time,
		public DTbase<DateTime>
{
	DateTime() { }
	
	DateTime(cchar* str)
	{
		convert(str);
	}
	
	DateTime(const ColData& str);

	DateTime(const std::string& str);

	short int compare(const DateTime& other) const;

	/// \brief Insert the date and time into a stream
	///
	/// The date and time are inserted into the stream, in that order,
	/// with a space between them.
	///
	/// \param os stream to insert date and time into
	std::ostream& out_stream(std::ostream& os) const;

	/// \brief Parse a MySQL date and time string into this object.
	cchar* convert(cchar*);
};


/// \brief Inserts a DateTime object into a C++ stream in a
/// MySQL-compatible format.
inline std::ostream& operator <<(std::ostream& s, const DateTime& d)
{
	return d.out_stream(s);
}


inline mysql_dt_base::operator std::string() const
{
	return stream2string<std::string>(*this);
}


inline Date::Date(const ColData& str)
{
	convert(str.c_str());
}


inline Date::Date(const std::string& str)
{
	convert(str.c_str());
}


inline Time::Time(const ColData& str)
{
	convert(str.c_str());
}


inline Time::Time(const std::string& str)
{
	convert(str.c_str());
}


inline DateTime::DateTime(const ColData& str)
{
	convert(str.c_str());
}


inline DateTime::DateTime(const std::string& str)
{
	convert(str.c_str());
}

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_DATETIME_H)
