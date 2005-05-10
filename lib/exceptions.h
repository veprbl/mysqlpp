/// \file exceptions.h
/// \brief Declares the MySQL++-specific exception classes.
///
/// When exceptions are enabled for a given
/// \ref mysqlpp::Connection "Connection" object, any of these exceptions
/// can be thrown as a result of operations done through that connection.

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

/// \brief Exception thrown when MySQL encounters a problem while
/// processing your query.
///
/// This is the most generic MySQL++ exception. It is thrown when your
/// SQL syntax is incorrect, or a field you requested doesn't exist in
/// the database, or....

class BadQuery : public std::exception {
public:
  BadQuery(const std::string &er = "") : error(er) {}
  ~BadQuery() throw () {}
  const std::string error; ///< contains explanation why query was bad
  virtual const char* what( void ) const throw () { return error.c_str(); }
};


/// \brief Exception thrown when a bad conversion takes place.

class BadConversion : public std::exception {
  const std::string _what;
public:
  const char*  type_name;
  const std::string data;
  size_t       retrieved;
  size_t       actual_size;
  BadConversion(const char* tn, const char* d, size_t r, size_t a)
    : _what(std::string("Tried to convert \"") + std::string(d ? d : "") + "\" to a \"" + std::string(tn ? tn : "")),
      type_name(tn), data(d), retrieved(r), actual_size(a) {};

  BadConversion(const std::string &wt, const char* tn, const char* d, size_t r, size_t a)
    : _what(wt), type_name(tn), data(d), retrieved(r), actual_size(a) {};

  BadConversion(const std::string& wt = "")
    : _what(wt), type_name("unknown"), data(""), retrieved(0), actual_size(0) {};
  ~BadConversion() throw () {}

  virtual const char* what( void ) const throw () { return _what.c_str(); }
};


/// \brief Exception thrown when you attempt to convert a SQL null
/// to an incompatible type.

class BadNullConversion : public std::exception {
  const std::string _what;
public:
  BadNullConversion(const std::string &wt = "") : _what(wt) {}
  ~BadNullConversion() throw () {}
  virtual const char* what( void ) const throw () { return _what.c_str(); }
};


/// \brief Exception thrown when not enough parameters are provided.
///
/// Thrown when not enough parameters are provided for a
/// template query.

class SQLQueryNEParms : public std::exception {
  const std::string _what;
public:
  SQLQueryNEParms(const char *c) : _what(std::string(c ? c : "")), error(c) {}
  ~SQLQueryNEParms() throw () {}
  const char* error;
  virtual const char* what( void ) const throw () { return _what.c_str(); }
};


/// \brief Exception thrown when a requested named field doesn't exist.
///
/// Thrown by Row::lookup_by_name() when you pass a field name that
/// isn't in the result set.

class BadFieldName : public std::exception {
  std::string _what;
public:
  BadFieldName(const char *bad_field)
	{
      _what = "Unknown field name: ";
      _what += bad_field;
    }
  ~BadFieldName() throw() {}
  virtual const char* what( void ) const throw () { return _what.c_str(); }
};

} // end namespace mysqlpp

#endif

