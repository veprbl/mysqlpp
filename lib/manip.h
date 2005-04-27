#ifndef MYSQLPP_MANIP_H
#define MYSQLPP_MANIP_H

/// \file manip.h
/// \brief Declares stream manipulators used with mysqlpp::Query.
///
/// These manipulators let you automatically quote elements inserted
/// into a Query stream or escape special characters in those elements.
/// This makes it easier to make syntactically correct SQL.

#include "defs.h"

#include "datetime.h"
#include "myset.h"
#include "sql_string.h"
#include "sql_query.h"

#include <mysql.h>

#include <iostream>

/// All global symbols in MySQL++ are in namespace mysqlpp.  This is
/// needed because many symbols are rather generic (e.g. Row, Query...),
/// so there is a serious danger of conflicts.
namespace mysqlpp {
	extern bool dont_quote_auto;

/// \enum quote_type0
/// The standard 'quote' manipulator.
///
/// Insert this into a stream to put single quotes around the next item
/// in the stream.  Useful for inserting string types into the database.

enum quote_type0 {
	quote	///< insert into a stream to single-quote next item
};

extern bool dont_quote_auto;

class SQLQueryParms;

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

struct quote_type1
{
  std::ostream *ostr;
  quote_type1(std::ostream *o) : ostr(o) {}
}; 

inline quote_type1 operator << (std::ostream &o, quote_type0 /*esc*/)
{
  return quote_type1(&o);
}

struct quote_type2
{
  SQLQueryParms *qparms;
  quote_type2(SQLQueryParms *p) : qparms(p) {}
};

inline quote_type2 operator << (SQLQueryParms &p, quote_type0 /*esc*/)
{
  return quote_type2(&p);
}

SQLQueryParms& operator << (quote_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (quote_type1 o, const T &in)
{
  return *o.ostr << in;
}

std::ostream&
operator << (std::ostream& o,const ColData_Tmpl<std::string>& in);

std::ostream&
operator << (std::ostream& o, const ColData_Tmpl<const_string>& in);

SQLQuery& operator << (SQLQuery& o, const ColData_Tmpl<std::string>& in);

SQLQuery& operator << (SQLQuery& o, const ColData_Tmpl<const_string>& in);

template <>
std::ostream& operator << (quote_type1 o, const std::string &in);

template <>
std::ostream& operator << (quote_type1 o, const char* const &in);

template <>
std::ostream& operator << (quote_type1 o, const ColData_Tmpl<std::string>& in);

template <>
std::ostream& operator << (quote_type1 o, const ColData_Tmpl<const_string>& in);

template <>
inline std::ostream& operator << (quote_type1 o, char* const &in) {
  return operator << (o, const_cast<const char* const &>(in));
}

template <>
inline std::ostream& operator << (quote_type1 o, const Date &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_type1 o, const Time &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_type1 o, const DateTime &in) {
  return *o.ostr << "'" << in << "'";
}

template <class ST>
inline std::ostream& operator << (quote_type1 o, const Set<ST> &in) {
  return *o.ostr << "'" << in << "'";
}

/// \endif


/// \enum quote_only_type0
/// The 'quote_only' manipulator.
///
/// Similar to 'quote' manipulator, but not sure what the distinction
/// is.  Docs needed!

enum quote_only_type0 { quote_only };

/// Documentation needed!
struct quote_only_type1 {
  std::ostream *ostr;
  quote_only_type1(std::ostream *o) : ostr(o) {}
}; 

inline quote_only_type1 operator << (std::ostream &o, quote_only_type0 /*esc*/) {
  return quote_only_type1(&o);
}

/// Documentation needed!
struct quote_only_type2 {
  SQLQueryParms *qparms;
  quote_only_type2(SQLQueryParms *p) : qparms(p) {}
};

inline quote_only_type2 operator << (SQLQueryParms &p, quote_only_type0 /*esc*/) {
  return quote_only_type2(&p);
}

SQLQueryParms & operator << (quote_only_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (quote_only_type1 o, const T &in) {
  return *o.ostr << in;
}

template <>
inline std::ostream& operator << (quote_only_type1 o, const std::string &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
std::ostream& operator << (quote_only_type1 o, const ColData_Tmpl<std::string>& in);

template <>
std::ostream& operator << (quote_only_type1 o, const ColData_Tmpl<const_string>& in);

template <>
inline std::ostream& operator << (quote_only_type1 o, const Date &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_only_type1 o, const Time &in) {
  return *o.ostr << "'" << in << "'";
}

template <>
inline std::ostream& operator << (quote_only_type1 o, const DateTime &in) {
  return *o.ostr << "'" << in << "'";
}

template <class ST>
inline std::ostream& operator << (quote_only_type1 o, const Set<ST> &in) {
  return *o.ostr << "'" << in << "'";
}


/// \enum quote_only_type0
/// The 'quote_only' manipulator.
///
/// Similar to 'quote' manipulator, but using double quotes instead of
/// single quotes.

enum quote_double_only_type0 { quote_double_only };

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

struct quote_double_only_type1 {
  std::ostream *ostr;
  quote_double_only_type1(std::ostream *o) : ostr(o) {}
}; 

inline quote_double_only_type1 operator << (std::ostream &o,
					    quote_double_only_type0 /*esc*/) {
  return quote_double_only_type1(&o);
}

struct quote_double_only_type2 {
  SQLQueryParms *qparms;
  quote_double_only_type2(SQLQueryParms *p) : qparms(p) {}
};

inline quote_double_only_type2 operator << (SQLQueryParms &p, 
					     quote_double_only_type0 /*esc*/) {
  return quote_double_only_type2(&p);
}

SQLQueryParms & operator << (quote_double_only_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (quote_double_only_type1 o, const T &in) {
  return *o.ostr << in;
}

template <>
inline std::ostream& operator << (quote_double_only_type1 o, const std::string &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <>
std::ostream& operator << (quote_double_only_type1 o, const ColData_Tmpl<std::string>& in);

template <>
std::ostream& operator << (quote_double_only_type1 o, const ColData_Tmpl<const_string>& in);

template <>
inline std::ostream& operator << (quote_double_only_type1 o, const Date &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <>
inline std::ostream& operator << (quote_double_only_type1 o, const Time &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <>
inline std::ostream& operator << (quote_double_only_type1 o,
			     const DateTime &in) {
  return *o.ostr << "\"" << in << "\"";
}

template <class ST>
inline std::ostream& operator << (quote_double_only_type1 o,
			     const Set<ST> &in) {
  return *o.ostr << "\"" << in << "\"";
}

/// \endif


/// \enum escape_type0
/// The 'escape' manipulator.
///
/// Calls mysql_escape_string() in the MySQL C API on the following
/// argument to prevent any special SQL characters from being
/// interpreted.

enum escape_type0 { escape };

/// Documentation needed!
struct escape_type1 {
  std::ostream *ostr;
  escape_type1(std::ostream *o) : ostr(o) {}
}; 

inline escape_type1 operator << (std::ostream &o, escape_type0 /*esc*/) {
  return escape_type1(&o);
}

/// Documentation needed!
struct escape_type2 {
  SQLQueryParms *qparms;
  escape_type2(SQLQueryParms *p) : qparms(p) {}
};

inline escape_type2 operator << (SQLQueryParms &p, escape_type0 /*esc*/) {
  return escape_type2(&p);
}

SQLQueryParms & operator << (escape_type2 p, SQLString &in);

template <class T>
inline std::ostream& operator << (escape_type1 o, const T &in) {
  return *o.ostr << in;
}

template <>
std::ostream& operator << (escape_type1 o, const std::string &in);

template <>
std::ostream& operator << (escape_type1 o, const char* const &in);

template <>
std::ostream& operator << (escape_type1 o, const ColData_Tmpl<std::string>& in);

template <>
std::ostream& operator << (escape_type1 o, const ColData_Tmpl<const_string>& in);

template <>
inline std::ostream& operator << (escape_type1 o, char* const &in) {
  return operator << (o, const_cast<const char* const &>(in));
}


/// \enum do_nothing_type0
/// The 'do_nothing' manipulator.
///
/// Not sure what this does.  Docs needed!

enum do_nothing_type0 { do_nothing };

/// \if INTERNAL
// Doxygen will not generate documentation for this section.

struct do_nothing_type1 {
  std::ostream *ostr;
  do_nothing_type1(std::ostream *o) : ostr(o) {}
}; 

inline do_nothing_type1 operator << (std::ostream &o, do_nothing_type0 /*esc*/) {
  return do_nothing_type1(&o);
}

template <class T>
inline std::ostream& operator << (do_nothing_type1 o, const T &in) {
  return *o.ostr << in;
}

struct do_nothing_type2 {
  SQLQueryParms *qparms;
  do_nothing_type2(SQLQueryParms *p) : qparms(p) {}
};

inline do_nothing_type2 operator << (SQLQueryParms &p, do_nothing_type0 /*esc*/) {
  return do_nothing_type2(&p);
}

inline SQLQueryParms & operator << (do_nothing_type2 p, SQLString &in) {
  in.processed = true;
  return *p.qparms << in;
}

/// \endif


/// \enum ignore_type0
/// The 'ignore' manipulator.
///
/// Not sure what this does.  Docs needed!

enum ignore_type0 { ignore };

/// Documentation needed!
struct ignore_type2 {
  SQLQueryParms *qparms;
  ignore_type2(SQLQueryParms *p) : qparms(p) {}
};

inline ignore_type2 operator << (SQLQueryParms &p, ignore_type0 /*esc*/) {
  return ignore_type2(&p);
}

inline SQLQueryParms & operator << (ignore_type2 p, SQLString &in) {
  return *p.qparms << in;
}

} // end namespace mysqlpp

#endif

