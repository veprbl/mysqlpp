/// \file exceptions.h
/// \brief Declares the MySQL++-specific exception classes.
///
/// When exceptions are enabled for a given mysqlpp::OptionalExceptions
/// derivative, any of these exceptions can be thrown on error.

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

#ifndef MYSQLPP_EXCEPTIONS_H
#define MYSQLPP_EXCEPTIONS_H

#include <exception>
#include <string>

namespace mysqlpp {

/// \brief Base class for all MySQL++ custom exceptions

class Exception : public std::exception
{
public:
	/// \brief Destroy exception object
	~Exception() throw() { }

	/// \brief Returns explanation of why exception was thrown
	virtual const char* what() const throw() { return what_.c_str(); }

protected:
	/// \brief Create exception object
	Exception(const char* w = "") throw() :
	what_(w)
	{
	}

	/// \brief Create exception object
	Exception(const std::string& w) throw() :
	what_(w)
	{
	}

	/// \brief explanation of why exception was thrown
	const std::string what_;
};


/// \brief Exception thrown when a bad type conversion is attempted.

class BadConversion : public Exception
{
public:
	const char* type_name;	///< name of type we tried to convert to
	const std::string data;	///< string form of data we tried to convert
	size_t retrieved;		///< documentation needed!
	size_t actual_size;		///< documentation needed!

	/// \brief Create exception object, building error string
	/// dynamically
	///
	/// \param tn type name we tried to convert to
	/// \param d string form of data we tried to convert
	/// \param r ??
	/// \param a ??
	BadConversion(const char* tn, const char* d, size_t r,
			size_t a) :
	Exception(std::string("Tried to convert \"") +
			std::string(d ? d : "") + "\" to a \"" +
			std::string(tn ? tn : "")),
	type_name(tn),
	data(d),
	retrieved(r),
	actual_size(a)
	{
	}

	/// \brief Create exception object, given completed error string
	///
	/// \param w the "what" error string
	/// \param tn type name we tried to convert to
	/// \param d string form of data we tried to convert
	/// \param r ??
	/// \param a ??
	BadConversion(const std::string& w, const char* tn,
				  const char* d, size_t r, size_t a) :
	Exception(w),
	type_name(tn),
	data(d),
	retrieved(r),
	actual_size(a)
	{
	}

	/// \brief Create exception object, with error string only
	///
	/// \param w the "what" error string
	///
	/// All other data members are initialize to default values
	BadConversion(const char* w = "") :
	Exception(w),
	type_name("unknown"),
	data(""),
	retrieved(0),
	actual_size(0)
	{
	}

	/// \brief Destroy exception
	~BadConversion() throw() { }
};


/// \brief Exception thrown when a requested named field doesn't exist.
///
/// Thrown by Row::lookup_by_name() when you pass a field name that
/// isn't in the result set.

class BadFieldName : public Exception
{
public:
	/// \brief Create exception object
	///
	/// \param bad_field name of field the MySQL server didn't like
	BadFieldName(const char* bad_field) :
	Exception(std::string("Unknown field name: ") + bad_field)
	{
	}

	/// \brief Destroy exception
	~BadFieldName() throw() { }
};


/// \brief Exception thrown when you attempt to convert a SQL null
/// to an incompatible type.

class BadNullConversion : public Exception
{
public:
	/// \brief Create exception object
	BadNullConversion(const char* w = "") :
	Exception(w)
	{
	}
};


/// \brief Exception thrown when not enough query parameters are
/// provided.
///
/// This is used in handling template queries.

class BadParamCount : public Exception
{
public:
	/// \brief Create exception object
	BadParamCount(const char* w = "") :
	Exception(w)
	{
	}

	/// \brief Destroy exception
	~BadParamCount() throw() { }
};


/// \brief Exception thrown when MySQL encounters a problem while
/// processing your query.
///
/// This is the most generic of MySQL++'s concrete exception types. It
/// is thrown when your SQL syntax is incorrect, or a field you
/// requested doesn't exist in the database, or....

class BadQuery : public Exception
{
public:
	/// \brief Create exception object
	BadQuery(const char* w = "") :
	Exception(w)
	{
	}
};


} // end namespace mysqlpp

#endif
