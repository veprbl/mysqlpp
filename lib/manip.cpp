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

/// \brief Inserts a SQLTypeAdapter into a stream, quoted and escaped
/// as appropriate to the data type the object was initialized from.

SQLQueryParms& operator <<(quote_type2 p, SQLTypeAdapter& in)
{
	if (in.quote_q()) {
		string temp("'", 1), escaped;
		p.qparms->escape_string(&escaped, in.data(), in.length());
		temp.append(escaped);
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
/// into a stream, quoted and escaped as needed if it's a Query stream

ostream& operator <<(quote_type1 o, const SQLTypeAdapter& in)
{
	Query* pq = dynamic_cast<Query*>(o.ostr);

	if (pq && in.quote_q()) o.ostr->write("'", 1);

	if (pq && in.escape_q()) {
		string escaped;
		pq->escape_string(&escaped, in.data(), in.length());
		o.ostr->write(escaped.data(), escaped.length());
	}
	else {
		o.ostr->write(in.data(), in.length());
	}

	if (pq && in.quote_q()) o.ostr->write("'", 1);

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


/// \brief Inserts a SQLTypeAdapter into a stream, quoting it unless it's
/// data that needs no quoting.
///
/// We make the decision to quote the data based on the in.quote_q()
/// flag.  You can set it yourself, but SQLTypeAdapter's ctors should set
/// it correctly for you.

SQLQueryParms& operator <<(quote_only_type2 p, SQLTypeAdapter& in)
{
	if (in.quote_q()) {
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


/// \brief Inserts a SQLTypeAdapter into a stream, double-quoting it (")
/// unless it's data that needs no quoting.
///
/// We make the decision to quote the data based on the in.quote_q()
/// flag.  You can set it yourself, but SQLTypeAdapter's ctors should set
/// it correctly for you.

SQLQueryParms& operator <<(quote_double_only_type2 p, SQLTypeAdapter& in)
{
	if (in.quote_q()) {
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


SQLQueryParms& operator <<(escape_type2 p, SQLTypeAdapter& in)
{
	if (in.escape_q()) {
		string escaped;
		p.qparms->escape_string(&escaped, in.data(), in.length());
		*p.qparms << SQLTypeAdapter(escaped, true);
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
	Query* pq = dynamic_cast<Query*>(o.ostr);

	if (pq && in.escape_q()) {
		string escaped;
		pq->escape_string(&escaped, in.data(), in.length());
		o.ostr->write(escaped.data(), escaped.length());
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

