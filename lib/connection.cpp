/***********************************************************************
 connection.cpp - Implements the Connection class.

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

#define MYSQLPP_NOT_HEADER
#include "platform.h"

#include "connection.h"

#include "query.h"
#include "result.h"

#if defined(HAVE_MYSQL_SHUTDOWN_LEVEL_ARG)
#	define SHUTDOWN_ARG ,SHUTDOWN_DEFAULT
#else
#	define SHUTDOWN_ARG
#endif

using namespace std;

namespace mysqlpp {

Connection::Connection(bool te) :
OptionalExceptions(te),
Lockable(true),
is_connected_(false),
success_(false)
{
	mysql_init(&mysql_);
}


Connection::Connection(const char* db, const char* host,
		const char* user, const char* passwd, uint port,
		my_bool compress, unsigned int connect_timeout,
		cchar* socket_name, unsigned int client_flag) :
OptionalExceptions(),
Lockable()
{
	mysql_init(&mysql_);
	if (connect(db, host, user, passwd, port, compress,
			connect_timeout, socket_name, client_flag)) {
		unlock();
		success_ = is_connected_ = true;
	}
	else {
		unlock();
		success_ = is_connected_ = false;
		if (throw_exceptions()) {
			throw ConnectionFailed(error());
		}
	}
}


Connection::~Connection()
{
	disconnect();
}


bool Connection::connect(cchar* db, cchar* host, cchar* user,
		cchar* passwd, uint port, my_bool compress,
		unsigned int connect_timeout, cchar* socket_name,
		unsigned int client_flag)
{
	mysql_.options.compress = compress;
	mysql_.options.connect_timeout = connect_timeout;

	lock();

	if (is_connected_) {
		disconnect();
	}

	mysql_options(&mysql_, MYSQL_READ_DEFAULT_FILE, "my");

	if (mysql_real_connect(&mysql_, host, user, passwd, db, port,
			socket_name, client_flag)) {
		unlock();
		success_ = is_connected_ = true;
	}
	else {
		unlock();
		success_ = is_connected_ = false;
		if (throw_exceptions()) {
			throw ConnectionFailed(error());
		}
	}

	if (success_ && db && db[0]) {
		success_ = select_db(db);
	}

	return success_;
}


void
Connection::disconnect()
{
	mysql_close(&mysql_);
	is_connected_ = false;
}


bool
Connection::create_db(const std::string& db)
{
	Query q(this, throw_exceptions());
	return q.exec("CREATE DATABASE " + db);
}


bool
Connection::drop_db(const std::string& db)
{
	Query q(this, throw_exceptions());
	return q.exec("DROP DATABASE " + db);
}


bool Connection::select_db(const char *db)
{
	bool suc = !(mysql_select_db(&mysql_, db));
	if (throw_exceptions() && !suc) {
		throw DBSelectionFailed(error());
	}
	else {
		return suc;
	}
}


bool Connection::reload()
{
	bool suc = !mysql_reload(&mysql_);
	if (throw_exceptions() && !suc) {
		// Reloading grant tables through this API isn't precisely a
		// query, but it's acceptable to signal errors with BadQuery
		// because the new mechanism is the FLUSH PRIVILEGES statement.
		// A program won't have to change when moving to the new way.
		throw BadQuery(error());
	}
	else {
		return suc;
	}
}


bool Connection::shutdown()
{
	bool suc = !(mysql_shutdown(&mysql_ SHUTDOWN_ARG));
	if (throw_exceptions() && !suc) {
		throw ConnectionFailed(error());
	}
	else {
		return suc;
	}
}


string Connection::info()
{
	const char* i = mysql_info(&mysql_);
	if (!i) {
		return string();
	}
	else {
		return string(i);
	}
}


Query Connection::query()
{
	return Query(this, throw_exceptions());
}

} // end namespace mysqlpp

