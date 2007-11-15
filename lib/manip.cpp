/***********************************************************************
 manip.cpp - Implements MySQL++'s various quoting/escaping stream
	manipulators.

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

#include "manip.h"

#include "query.h"

using namespace std;

// Manipulator stuff is _always_ in namespace mysqlpp.
namespace mysqlpp {

/// \brief Set to true if you want to suppress automatic quoting
///
/// Works only for String inserted into C++ streams.

bool dont_quote_auto = false;


/// \brief Inserts a SQLTypeAdapter into a stream, quoted and escaped
/// as appropriate to the data type the object was initialized from.

SQLQueryParms& operator <<(quote_type2 p, SQLTypeAdapter& in)
{
	if (in.is_string()) {
		string temp("'", 1);
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());
		temp.append(escaped, len);
		delete[] escaped;
		temp.append("'", 1);
		*p.qparms << SQLTypeAdapter(temp, true);
		return *p.qparms;
	}
	else {
		in.set_processed();
		return *p.qparms << in;
	}
}


/// \brief Inserts a anything that can be converted to SQLTypeAdapter
/// into a stream, quoted and escaped as needed

ostream& operator <<(quote_type1 o, const SQLTypeAdapter& in)
{
	if (in.is_string()) {
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());

		o.ostr->write("'", 1);
		o.ostr->write(escaped, len);
		o.ostr->write("'", 1);

		delete[] escaped;
	}
	else {
		o.ostr->write(in.data(), in.length());
	}

	return *o.ostr;
}


/// \brief Inserts a String into a stream, quoted and escaped as needed
///
/// Because String was designed to contain MySQL type data, we may
/// choose not to actually quote or escape the data, if it is not
/// needed.

ostream& operator <<(quote_type1 o, const String& in)
{
	if (in.escape_q()) {
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());

		if (in.quote_q()) o.ostr->write("'", 1);
		o.ostr->write(escaped, len);
		if (in.quote_q()) o.ostr->write("'", 1);

		delete[] escaped;
	}
	else {
		if (in.quote_q()) o.ostr->write("'", 1);
		o.ostr->write(in.data(), in.length());
		if (in.quote_q()) o.ostr->write("'", 1);
	}

	return *o.ostr;
}


/// \brief Inserts a SQLTypeAdapter into a non-Query stream.
///
/// Although we know how to quote and escape SQLTypeAdapter objects, we
/// only do that when inserting them into Query streams or when given an
/// explicit manipulator because this feature is only intended to make
/// it easier to build syntactically-correct SQL queries.

ostream&
operator <<(ostream& o, const SQLTypeAdapter& in)
{
	o.write(in.data(), in.length());
	return o;
}


/// \brief Inserts a String into a non-Query stream.
///
/// Although we know how to automatically quote and escape String
/// objects, we only do that when inserting them into Query streams
/// because this feature is only intended to make it easier to build
/// syntactically-correct SQL queries.  You can force the library to
/// give you quoting and escaping with the quote manipulator:
///
/// \code
/// mysqlpp::String cd("...");
/// cout << mysqlpp::quote << cd << endl;
/// \endcode

ostream& operator <<(ostream& o, const String& in)
{
	o.write(in.data(), in.length());
	return o;
}


/// \brief Insert a String into a Query stream
///
/// This operator appears to be a workaround for a weakness in one
/// compiler's implementation of the C++ type system.  See Wishlist for
/// current plan on what to do about this.

Query& operator <<(Query& o, const String& in)
{
	if (dont_quote_auto) {
		o.write(in.data(), in.length());
	}
	else if (in.escape_q()) {
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());

		if (in.quote_q()) o.write("'", 1);
		o.write(escaped, len);
		if (in.quote_q()) o.write("'", 1);

		delete[] escaped;
	}
	else {
		if (in.quote_q()) o.write("'", 1);
		o.write(in.data(), in.length());
		if (in.quote_q()) o.write("'", 1);
	}

	return o;
}


/// \brief Inserts a SQLTypeAdapter into a stream, quoting it unless it's
/// data that needs no quoting.
///
/// We make the decision to quote the data based on the in.is_string()
/// flag.  You can set it yourself, but SQLTypeAdapter's ctors should set
/// it correctly for you.

SQLQueryParms& operator <<(quote_only_type2 p, SQLTypeAdapter& in)
{
	if (in.is_string()) {
		string temp("'", 1);
		temp.append(in.data(), in.length());
		temp.append("'", 1);
		return *p.qparms << SQLTypeAdapter(temp, true);
	}
	else {
		in.set_processed();
		return *p.qparms << in;
	}
}


/// \brief Inserts a String into a stream, quoted as needed
///
/// Because String was designed to contain MySQL type data, we may
/// choose not to actually quote the data, if it is not needed.

ostream& operator <<(quote_only_type1 o, const String& in)
{
	if (in.quote_q()) o.ostr->write("'", 1);
	o.ostr->write(in.data(), in.length());
	if (in.quote_q()) o.ostr->write("'", 1);

	return *o.ostr;
}


/// \brief Inserts a SQLTypeAdapter into a stream, double-quoting it (")
/// unless it's data that needs no quoting.
///
/// We make the decision to quote the data based on the in.is_string()
/// flag.  You can set it yourself, but SQLTypeAdapter's ctors should set
/// it correctly for you.

SQLQueryParms& operator <<(quote_double_only_type2 p, SQLTypeAdapter& in)
{
	if (in.is_string()) {
		string temp("\"", 1);
		temp.append(in.data(), in.length());
		temp.append("\"", 1);
		return *p.qparms << SQLTypeAdapter(temp, true);
	}
	else {
		in.set_processed();
		return *p.qparms << in;
	}
}


/// \brief Inserts a String into a stream, double-quoted (") as needed
///
/// Because String was designed to contain MySQL type data, we may
/// choose not to actually quote the data, if it is not needed.

ostream& operator <<(quote_double_only_type1 o, const String& in)
{
	if (in.quote_q()) o.ostr->write("\"", 1);
	o.ostr->write(in.data(), in.length());
	if (in.quote_q()) o.ostr->write("\"", 1);

	return *o.ostr;
}


SQLQueryParms& operator <<(escape_type2 p, SQLTypeAdapter& in)
{
	if (in.is_string()) {
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());

		*p.qparms << SQLTypeAdapter(escaped, len, true);

		delete[] escaped;
		return *p.qparms;
	}
	else {
		in.set_processed();
		return *p.qparms << in;
	}
}


/// \brief Inserts anything that can be converted to SQLTypeAdapter into
/// a stream, escaping special SQL characters as needed.

std::ostream& operator <<(escape_type1 o, const SQLTypeAdapter& in)
{
	if (in.is_string()) {
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());
		o.ostr->write(escaped, len);
		delete[] escaped;
	}
	else {
		o.ostr->write(in.data(), in.length());
	}

	return *o.ostr;
}


/// \brief Inserts a String into a stream, escaping special SQL characters
///
/// Because String was designed to contain MySQL type data, we may
/// choose not to escape the data, if it is not needed.

std::ostream& operator <<(escape_type1 o, const String& in)
{
	if (in.escape_q()) {
		char* escaped = new char[in.length() * 2 + 1];
		size_t len = mysql_escape_string(escaped, in.data(), in.length());
		o.ostr->write(escaped, len);
		delete[] escaped;
	}
	else {
		o.ostr->write(in.data(), in.length());
	}

	return *o.ostr;
}


/// \brief Inserts a SQLTypeAdapter into a stream, with no escaping or
/// quoting.

SQLQueryParms& operator <<(do_nothing_type2 p, SQLTypeAdapter& in)
{
	in.set_processed();
	return *p.qparms << in;
}


/// \brief Inserts a SQLTypeAdapter into a stream, with no escaping or
/// quoting, and without marking the string as having been "processed".

SQLQueryParms& operator <<(ignore_type2 p, SQLTypeAdapter& in)
{
	return *p.qparms << in;
}

} // end namespace mysqlpp

