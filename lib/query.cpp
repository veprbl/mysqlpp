/***********************************************************************
 query.cpp - Implements the Query class.

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

#include "query.h"

#include "autoflag.h"
#include "connection.h"

namespace mysqlpp {

Query::Query(Connection* c, bool te, const char* qstr) :
#if defined(_MSC_VER) && !defined(_STLP_VERSION) && !defined(_STLP_VERSION_STR)
// prevents a double-init memory leak in native VC++ RTL (not STLport!)
std::ostream(std::_Noinit), 
#else
std::ostream(0),
#endif
OptionalExceptions(te),
template_defaults(this),
conn_(c),
copacetic_(true)
{
	init(&sbuffer_);
	if (qstr) {
		sbuffer_.str(qstr);
	}
}

Query::Query(const Query& q) :
#if defined(_MSC_VER) && !defined(_STLP_VERSION) && !defined(_STLP_VERSION_STR)
// ditto above
std::ostream(std::_Noinit),
#else
std::ostream(0),
#endif
OptionalExceptions(q.throw_exceptions()),
template_defaults(q.template_defaults),
conn_(q.conn_),
copacetic_(q.copacetic_)
{
	init(&sbuffer_);
}


Query&
Query::operator=(const Query& rhs)
{
	set_exceptions(rhs.throw_exceptions());
	template_defaults = rhs.template_defaults;
	conn_ = rhs.conn_;
	copacetic_ = rhs.copacetic_;

	return *this;
}


my_ulonglong
Query::affected_rows() const
{
	return conn_->affected_rows();
}


size_t
Query::escape_string(std::string* ps, const char* original,
		size_t length) const
{
	if (ps == 0) {
		// Can't do any real work!
		return 0;
	}
	else if (original == 0) {
		// ps must point to the original data as well as to the
		// receiving string, so get the pointer and the length from it.
		original = ps->data();
		length = ps->length();
	}
	else if (length == 0) {
		// We got a pointer to a C++ string just for holding the result
		// and also a C string pointing to the original, so find the
		// length of the original.
		length = strlen(original);
	}

	char* escaped = new char[length * 2 + 1];
	length = escape_string(escaped, original, length);
	ps->assign(escaped, length);
	delete[] escaped;

	return length;
}


size_t
Query::escape_string(char* escaped, const char* original,
		size_t length) const
{
	if (conn_ && *conn_) {
		// Normal case
		return mysql_real_escape_string(&conn_->mysql_, escaped,
				original, length);
	}
	else {
		// Should only happen in test/test_manip.cpp, since it doesn't
		// want to open a DB connection just to test the manipulators.
		return mysql_escape_string(escaped, original, length);
	}
}


bool
Query::exec(const std::string& str)
{
	copacetic_ = !mysql_real_query(&conn_->mysql_, str.data(),
			static_cast<unsigned long>(str.length()));

	if (parse_elems_.size() == 0) {
		// not a template query, so auto-reset
		reset();
	}

	if (!copacetic_ && throw_exceptions()) {
		throw BadQuery(error(), errnum());
	}
	else {
		return copacetic_;
	}
}


ResNSel
Query::execute(SQLQueryParms& p)
{
	AutoFlag<> af(template_defaults.processing_);
	return execute(str(p));
}


ResNSel
Query::execute(const SQLTypeAdapter& s)
{
	if ((parse_elems_.size() == 2) && !template_defaults.processing_) {
		// We're a template query and this isn't a recursive call, so
		// take s to be a lone parameter for the query.  We will come
		// back in here with a completed query, but the processing_
		// flag will be set, allowing us to avoid an infinite loop.
		AutoFlag<> af(template_defaults.processing_);
		return execute(SQLQueryParms() << s);
	}
	else {
		// Take s to be the entire query string
		return execute(s.data(), s.length());
	}
}


ResNSel
Query::execute(const char* str, size_t len)
{
	copacetic_ = !mysql_real_query(&conn_->mysql_, str, len);

	if (parse_elems_.size() == 0) {
		// Not a template query, so auto-reset
		reset();
	}

	if (copacetic_) {
		return ResNSel(conn_, conn_->insert_id(),
				conn_->affected_rows(), conn_->info());
	}
	else if (throw_exceptions()) {
		throw BadQuery(error(), errnum());
	}
	else {
		return ResNSel();
	}
}


std::string
Query::info()
{
	return conn_->info();
}


my_ulonglong
Query::insert_id()
{
	return conn_->insert_id();
}


bool 
Query::more_results()
{
#if MYSQL_VERSION_ID > 41000		// only in MySQL v4.1 +
	return mysql_more_results(&conn_->mysql_);
#else
	return false;
#endif
}


void
Query::parse()
{
	std::string str = "";
	char num[4];
	std::string name;
	char *s, *s0;
	s0 = s = preview_char();
	while (*s) {
		if (*s == '%') {
			// Following might be a template parameter declaration...
			s++;
			if (*s == '%') {
				// Doubled percent sign, so insert literal percent sign.
				str += *s++;
			}
			else if (isdigit(*s)) {
				// Number following percent sign, so it signifies a
				// positional parameter.  First step: find position
				// value, up to 3 digits long.
				num[0] = *s;
				s++;
				if (isdigit(*s)) {
					num[1] = *s;
					num[2] = 0;
					s++;
					if (isdigit(*s)) {
						num[2] = *s;
						num[3] = 0;
						s++;
					}
					else {
						num[2] = 0;
					}
				}
				else {
					num[1] = 0;
				}
				signed char n = atoi(num);

				// Look for option character following position value.
				char option = ' ';
				if (*s == 'q' || *s == 'Q') {
					option = *s++;
				}

				// Is it a named parameter?
				if (*s == ':') {
					// Save all alphanumeric and underscore characters
					// following colon as parameter name.
					s++;
					for (/* */; isalnum(*s) || *s == '_'; ++s) {
						name += *s;
					}

					// Eat trailing colon, if it's present.
					if (*s == ':') {
						s++;
					}

					// Update maps that translate parameter name to
					// number and vice versa.
					if (n >= static_cast<short>(parsed_names_.size())) {
						parsed_names_.insert(parsed_names_.end(),
								static_cast<std::vector<std::string>::size_type>(
										n + 1) - parsed_names_.size(),
								std::string());
					}
					parsed_names_[n] = name;
					parsed_nums_[name] = n;
				}

				// Finished parsing parameter; save it.
				parse_elems_.push_back(SQLParseElement(str, option, n));
				str = "";
				name = "";
			}
			else {
				// Insert literal percent sign, because sign didn't
				// precede a valid parameter string; this allows users
				// to play a little fast and loose with the rules,
				// avoiding a double percent sign here.
				str += '%';
			}
		}
		else {
			// Regular character, so just copy it.
			str += *s++;
		}
	}

	parse_elems_.push_back(SQLParseElement(str, ' ', -1));
	delete[] s0;
}


SQLTypeAdapter*
Query::pprepare(char option, SQLTypeAdapter& S, bool replace)
{
	if (S.is_processed()) {
		return &S;
	}

	if (option == 'q') {
		std::string temp(S.quote_q() ? "'" : "", S.quote_q() ? 1 : 0);

		if (S.escape_q()) {
			char *escaped = new char[S.size() * 2 + 1];
			size_t len = mysql_real_escape_string(&conn_->mysql_, escaped,
					S.data(), static_cast<unsigned long>(S.size()));
			temp.append(escaped, len);
			delete[] escaped;
		}
		else {
			temp.append(S.data(), S.length());
		}

		if (S.quote_q()) temp.append("'", 1);

		SQLTypeAdapter* ss = new SQLTypeAdapter(temp);

		if (replace) {
			S = *ss;
			S.set_processed();
			delete ss;
			return &S;
		}
		else {
			return ss;
		}
	}
	else if (option == 'Q' && S.quote_q()) {
		std::string temp("'", 1);
		temp.append(S.data(), S.length());
		temp.append("'", 1);
		SQLTypeAdapter *ss = new SQLTypeAdapter(temp);

		if (replace) {
			S = *ss;
			S.set_processed();
			delete ss;
			return &S;
		}
		else {
			return ss;
		}
	}
	else {
		if (replace) {
			S.set_processed();
		}
		return &S;
	}
}


char*
Query::preview_char()
{
	const std::string& str(sbuffer_.str());
	char* s = new char[str.size() + 1];
	memcpy(s, str.data(), str.size()); 
	s[str.size()] = '\0';
	return s;
}


void
Query::proc(SQLQueryParms& p)
{
	sbuffer_.str("");

	for (std::vector<SQLParseElement>::iterator i = parse_elems_.begin();
			i != parse_elems_.end(); ++i) {
		MYSQLPP_QUERY_THISPTR << i->before;
		int num = i->num;
		if (num >= 0) {
			SQLQueryParms* c;
			if (size_t(num) < p.size()) {
				c = &p;
			}
			else if (size_t(num) < template_defaults.size()) {
				c = &template_defaults;
			}
			else {
				*this << " ERROR";
				throw BadParamCount(
						"Not enough parameters to fill the template.");
			}

			SQLTypeAdapter& param = (*c)[num];
			SQLTypeAdapter* ss = pprepare(i->option, param, c->bound());
			MYSQLPP_QUERY_THISPTR << *ss;
			if (ss != &param) {
				// pprepare() returned a new string object instead of
				// updating param in place, so we need to delete it.
				delete ss;
			}
		}
	}
}


void
Query::reset()
{
	seekp(0);
	clear();
	sbuffer_.str("");

	parse_elems_.clear();
	template_defaults.clear();
}


Result
Query::store(SQLQueryParms& p)
{
	AutoFlag<> af(template_defaults.processing_);
	return store(str(p));
}


Result 
Query::store(const SQLTypeAdapter& s)
{
	if ((parse_elems_.size() == 2) && !template_defaults.processing_) {
		// We're a template query and this isn't a recursive call, so
		// take s to be a lone parameter for the query.  We will come
		// back in here with a completed query, but the processing_
		// flag will be set, allowing us to avoid an infinite loop.
		AutoFlag<> af(template_defaults.processing_);
		return store(SQLQueryParms() << s);
	}
	else {
		// Take s to be the entire query string
		return store(s.data(), s.length());
	}
}


Result
Query::store(const char* str, size_t len)
{
	copacetic_ = !mysql_real_query(&conn_->mysql_, str, len);

	if (parse_elems_.size() == 0) {
		// Not a template query, so auto-reset
		reset();
	}

	MYSQL_RES* res = 0;
	if (copacetic_) {
		res = mysql_store_result(&conn_->mysql_);
	}

	if (res) {
		return Result(res, throw_exceptions());
	}
	else {
		// Either result set is empty (which is copacetic), or there
		// was a problem returning the result set (which is not).  If
		// caller knows the result set will be empty (e.g. query is
		// INSERT, DELETE...) it should call exec{ute}() instead, but
		// there are good reasons for it to be unable to predict this.
		copacetic_ = mysql_field_count(&conn_->mysql_) == 0;
		if (copacetic_ || !throw_exceptions()) {
			return Result();
		}
		else {
			throw BadQuery(error(), errnum());
		}
	}
}


Result
Query::store_next()
{
#if MYSQL_VERSION_ID > 41000		// only in MySQL v4.1 +
	int ret;
	if ((ret = mysql_next_result(&conn_->mysql_)) == 0) {
		// There are more results, so return next result set.
		MYSQL_RES* res = mysql_store_result(&conn_->mysql_);
		if (res) {
			return Result(res, throw_exceptions());
		} 
		else {
			// Result set is null, but throw an exception only i it is
			// null because of some error.  If not, it's just an empty
			// result set, which is harmless.  We return an empty result
			// set if exceptions are disabled, as well.
			if (conn_->errnum() && throw_exceptions()) {
				throw BadQuery(error(), errnum());
			} 
			else {
				return Result();
			}
		}
	}
	else if (throw_exceptions()) {
        if (ret > 0) {
            throw BadQuery(error(), errnum());
        }
        else {
            throw EndOfResultSets();
        }
    }
    else {
        return Result();
	}
#else
	return store();
#endif // MySQL v4.1+
}


std::string
Query::str(SQLQueryParms& p)
{
	if (!parse_elems_.empty()) {
		proc(p);
	}

	return sbuffer_.str();
}


ResUse
Query::use(SQLQueryParms& p)
{
	AutoFlag<> af(template_defaults.processing_);
	return use(str(p));
}


ResUse
Query::use(const SQLTypeAdapter& s)
{
	if ((parse_elems_.size() == 2) && !template_defaults.processing_) {
		// We're a template query and this isn't a recursive call, so
		// take s to be a lone parameter for the query.  We will come
		// back in here with a completed query, but the processing_
		// flag will be set, allowing us to avoid an infinite loop.
		AutoFlag<> af(template_defaults.processing_);
		return use(SQLQueryParms() << s);
	}
	else {
		// Take s to be the entire query string
		return use(s.data(), s.length());
	}
}


ResUse
Query::use(const char* str, size_t len)
{
	copacetic_ = !mysql_real_query(&conn_->mysql_, str, len);

	if (parse_elems_.size() == 0) {
		// Not a template query, so auto-reset
		reset();
	}

	MYSQL_RES* res = 0;
	if (copacetic_) {
		res = mysql_use_result(&conn_->mysql_);
	}

	if (res) {
		return ResUse(res, throw_exceptions());
	}
	else {
		// Either result set is empty (which is copacetic), or there
		// was a problem returning the result set (which is not).  If
		// caller knows the result set will be empty (e.g. query is
		// INSERT, DELETE...) it should call exec{ute}() instead, but
		// there are good reasons for it to be unable to predict this.
		copacetic_ = mysql_field_count(&conn_->mysql_) == 0;
		if (copacetic_ || !throw_exceptions()) {
			return ResUse();
		}
		else {
			throw BadQuery(error(), errnum());
		}
	}
}


} // end namespace mysqlpp

