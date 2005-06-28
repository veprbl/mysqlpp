/// \file sql_query.h
/// \brief Declares the base class for mysqlpp::Query.
///

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

#ifndef MYSQLPP_SQL_QUERY_H
#define MYSQLPP_SQL_QUERY_H

#include "defs.h"
#include "qparms.h"
#include "sql_string.h"
#include "tiny_int.h"

#include <sstream>
#include <vector>
#include <map>

/// \brief Used to define many similar functions in class SQLQuery.
#define mysql_query_define0(RETURN, FUNC)\
  RETURN FUNC (ss a)\
    {return FUNC (parms() << a);}\
  RETURN FUNC (ss a, ss b)\
    {return FUNC (parms() << a << b);}\
  RETURN FUNC (ss a, ss b, ss c)\
    {return FUNC (parms() << a << b << c);}\
  RETURN FUNC (ss a, ss b, ss c, ss d)\
    {return FUNC (parms() << a << b << c << d);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e)\
    {return FUNC (parms() << a << b << c << d << e);} \
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f)\
    {return FUNC (parms() << a << b << c << d << e << f);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g)\
    {return FUNC (parms() << a << b << c << d << e << f << g);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h)\
    {return FUNC (parms() << a << b << c << d << e << f << g << h);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)\
    {return FUNC (parms() << a << b << c << d << e << f << g << h << i);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k,\
		       ss l)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k <<l);}\

namespace mysqlpp {

/// \brief Used for indicating whether a query object should auto-reset
enum query_reset { DONT_RESET, RESET_QUERY };

class SQLQuery : public std::stringstream
{
private:
	friend class SQLQueryParms;

	char *preview_char();

protected:
	bool Success;					///< if true, last query succeeded
	char* errmsg;					///< string explaining last query error

	/// \brief List of template query parameters
	std::vector<SQLParseElement> parsed;

	/// \brief Maps template parameter position values to the
	/// corresponding parameter name.
	std::vector<std::string> parsed_names;

	/// \brief Maps template parameter names to their position value.
	std::map<std::string, int> parsed_nums;

	typedef const SQLString& ss;	///< to keep parameters lists short
	typedef SQLQueryParms parms;	///< abstraction; remove when Query and SQLQuery merge

	/// \brief Process a parameterized query list.
	void proc(SQLQueryParms& p);

public:
	/// \brief Default constructor
	SQLQuery() :
	std::stringstream(),
	Success(false),
	errmsg(0),
	def(this)
	{
	}
	
	/// \brief Create a query as a copy of another
	SQLQuery(const SQLQuery& q);

	/// \brief The default template parameters
	///
	/// Used for filling in parameterized queries.
	SQLQueryParms def;

	/// \brief Treat the contents of the query string as a template
	/// query.
	///
	/// This method sets up the internal structures used by all of the
	/// other members that accept template query parameters.  See the
	/// "Template Queries" chapter in the user manual for more
	/// information.
	void parse();

	/// \brief Return the last error message that was set
	///
	/// This function has no real meaning at this level.  It's
	/// overridden with useful behavior in the Query subclass.
	std::string error() const
	{
		return errmsg;
	}
	
	/// \brief Return true if the last query was successful
	bool success() const { return Success; }

	/// \brief Return true if the last query was successful
	operator bool() { return success(); }

	/// \brief Return true if the last query failed
	bool operator !() { return !success(); }

	/// \brief Reset the query object so that it can be reused.
	///
	/// This erases the query string and the contents of the parameterized
	/// query element list.
	void reset();

	/// \brief Build an UPDATE SQL query
	///
	/// \param o SSQLS containing data to match in table
	/// \param n SSQLS that will replace any records in \c o.table() that
	/// match \c o
	template <class T>
	SQLQuery& update(const T& o, const T& n)
	{
		reset();

		// Cast required for VC++ 2003 due to error in overloaded operator
		// lookup logic.  For an explanation of the problem, see:
		// http://groups-beta.google.com/group/microsoft.public.vc.stl/browse_thread/thread/9a68d84644e64f15
		dynamic_cast<std::stringstream&>(*this) << "UPDATE " <<
				o.table() << " SET " << n.equal_list() << " WHERE " <<
				o.equal_list(" AND ", sql_use_compare);
		return *this;
	}

	/// \brief Build an INSERT SQL query for one record
	///
	/// \param v SSQLS which will be inserted into \c v.table()
	template <class T>
	SQLQuery& insert(const T& v)
	{
		reset();

		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "INSERT INTO " <<
				v.table() << " (" << v.field_list() << ") VALUES (" <<
				v.value_list() << ")";
		return *this;
	}

	/// \brief Build an INSERT SQL query for multiple records
	///
	/// \param first iterator pointing to first SSQLS to insert
	/// \param last iterator pointing to record past last SSQLS to
	/// insert
	template <class Iter>
	SQLQuery& insert(Iter first, Iter last)
	{
		reset();
		if (first == last) {
			return *this;	// empty set!
		}
		
		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "INSERT INTO " <<
				first->table() << " (" << first->field_list() <<
				") VALUES (" << first->value_list() << ')';

		Iter it = first + 1;
		while (it != last) {
			dynamic_cast<std::stringstream&>(*this) << ",(" <<
					it->value_list() << ')';
			++it;
		}

		return *this;
	} 

	/// \brief Build a REPLACE SQL query
	///
	/// \param v SSQLS containing data to insert into the table, or
	/// which will replace an existing record if this record matches
	/// another on a unique index
	template <class T>
	SQLQuery& replace(const T& v)
	{
		reset();

		// See above comment for cast rationale
		dynamic_cast<std::stringstream&>(*this) << "REPLACE INTO " <<
				v.table() << " (" << v.field_list() << ") VALUES (" <<
				v.value_list() << ")";
		return *this;
	}

	/// \brief Get built query as a null-terminated C++ string
	std::string str()
	{
		return str(def);
	}

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param r if equal to \c RESET_QUERY, query object is cleared
	/// after this call
	std::string str(query_reset r)
	{
		return str(def, r);
	}

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	std::string str(SQLQueryParms& p);

	/// \brief Get built query as a null-terminated C++ string
	///
	/// \param p template query parameters to use, overriding the ones
	/// this object holds, if any
	/// \param r if equal to \c RESET_QUERY, query object is cleared
	/// after this call
	std::string str(SQLQueryParms& p, query_reset r);

#if !defined(DOXYGEN_IGNORE)
	// Make Doxygen ignore this macro; declaration below is sufficient.
	mysql_query_define0(std::string, str);
#endif // !defined(DOXYGEN_IGNORE)
};

} // end namespace mysqlpp

#endif

