/***********************************************************************
 query.cpp - Implements the Query class.

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

#include "query.h"

namespace mysqlpp {

Query::Query(const Query& q) :
SQLQuery(q),
OptionalExceptions(q.exceptions()),
conn_(q.conn_)
{
}


bool Query::exec(const std::string& str)
{
	Success = !mysql_real_query(&conn_->mysql, str.c_str(),
			str.length());
	if (!Success && throw_exceptions()) {
		throw BadQuery(conn_->error());
	}
	else {
		return Success;
	}
}


bool Query::success()
{
	if (Success) {
		return conn_->success();
	}
	else {
		return false;
	}
}


/// \if INTERNAL
// Doxygen will not generate documentation for this section.

ResNSel Query::execute(const char* str)
{
	Success = false;
	if (lock()) {
		if (throw_exceptions()) {
			throw BadQuery("lock failed");
		}
		else {
			return ResNSel();
		}
	}

	Success = !mysql_query(&conn_->mysql, str);
	unlock();
	if (Success) {
		return ResNSel(conn_);
	}
	else {
		if (throw_exceptions()) {
			throw BadQuery(conn_->error());
		}
		else {
			return ResNSel();
		}
	}
}


ResNSel Query::execute(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return execute(str(p, r).c_str());
}


ResUse Query::use(const char* str)
{
	Success = false;
	if (lock()) {
		if (throw_exceptions()) {
			throw BadQuery("lock failed");
		}
		else {
			return ResUse();
		}
	}

	Success = !mysql_query(&conn_->mysql, str);
	if (Success) {
		MYSQL_RES* res = mysql_use_result(&conn_->mysql);
		if (res) {
			unlock();
			return ResUse(res, conn_, throw_exceptions());
		}
	}
	unlock();

	// One of the mysql_* calls failed, so decide how we should fail.
	if (throw_exceptions()) {
		throw BadQuery(conn_->error());
	}
	else {
		return ResUse();
	}
}


ResUse Query::use(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return use(str(p, r).c_str());
}


Result Query::store(const char* str)
{
	Success = false;

	if (lock()) {
		if (throw_exceptions()) {
			throw BadQuery("lock failed");
		}
		else {
			return Result();
		}
	}

	Success = !mysql_query(&conn_->mysql, str);
	if (Success) {
		MYSQL_RES* res = mysql_store_result(&conn_->mysql);
		if (res) {
			unlock();
			return Result(res);
		}
	}
	unlock();

	// One of the mysql_* calls failed, so decide how we should fail.
	if (throw_exceptions()) {
		throw BadQuery(conn_->error());
	}
	else {
		return Result();
	}
}


Result Query::store(parms& p)
{
	query_reset r = parsed.size() ? DONT_RESET : RESET_QUERY;
	return store(str(p, r).c_str());
}

/// \endif


my_ulonglong Query::affected_rows() const
{
	return conn_->affected_rows();
}


my_ulonglong Query::insert_id()
{
	return conn_->insert_id();
}


std::string Query::info()
{
	return conn_->info();
}


std::string Query::error()
{
	if (errmsg) {
		return std::string(errmsg);
	}
	else {
		return conn_->error();
	}
}


bool Query::lock()
{
    return conn_->lock();
}


void Query::unlock()
{
	conn_->unlock();
}

} // end namespace mysqlpp

