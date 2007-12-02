/***********************************************************************
 dbdriver.cpp - Implements the DBDriver class.

 Copyright (c) 2005-2007 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS file in the top directory of the distribution for details.

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
#include "dbdriver.h"

// An argument was added to mysql_shutdown() in MySQL 4.1.3 and 5.0.1.
#if ((MYSQL_VERSION_ID >= 40103) && (MYSQL_VERSION_ID <= 49999)) || (MYSQL_VERSION_ID >= 50001)
#	define SHUTDOWN_ARG ,SHUTDOWN_DEFAULT
#else
#	define SHUTDOWN_ARG
#endif

using namespace std;

namespace mysqlpp {

DBDriver::DBDriver() :
is_connected_(false)
{
	mysql_init(&mysql_);
}


DBDriver::DBDriver(const DBDriver& other) :
is_connected_(false)
{
	copy(other);
}


DBDriver::~DBDriver()
{
	if (connected()) {
		disconnect();
	}
}


bool
DBDriver::connect(const char* host, const char* socket_name,
		unsigned int port, const char* db, const char* user,
		const char* password)
{
	// Drop previous connection, if any
	if (connected()) {
		disconnect();
	}

	// Set defaults for connection options.  User can override these
	// by calling set_option() before connect().
	set_option_default(option::read_default_file, "my");

	// Establish the connection
	return is_connected_ =
			mysql_real_connect(&mysql_, host, user, password, db,
			port, socket_name, mysql_.client_flag);
}


bool
DBDriver::connect(const MYSQL& other)
{
	// Drop previous connection, if any
	if (connected()) {
		disconnect();
	}

	// Set defaults for connection options.  User can override these
	// by calling set_option() before connect().
	set_option_default(option::read_default_file, "my");

	// Establish the connection
	return is_connected_ =
			mysql_real_connect(&mysql_, other.host, other.user,
			other.passwd, other.db, other.port, other.unix_socket,
			other.client_flag);
}


void
DBDriver::copy(const DBDriver& other)
{
	if (other.connected()) {
		connect(other.mysql_);
	}
	else {
		is_connected_ = false;
	}
}


void
DBDriver::disconnect()
{
	mysql_close(&mysql_);
	is_connected_ = false;
}


const char*
DBDriver::enable_ssl(const char* key, const char* cert,
		const char* ca, const char* capath, const char* cipher)
{
#if defined(HAVE_MYSQL_SSL_SET)
	mysql_ssl_set(&mysql_, key, cert, ca, capath, cipher);
	return "";
#else
	return "SSL not enabled in MySQL++";
#endif
}


DBDriver&
DBDriver::operator=(const DBDriver& rhs)
{
	copy(rhs);
	return *this;
}


bool
DBDriver::option_set(option::Type o) const
{
	for (option::ListIt it = applied_options_.begin();
			it != applied_options_.end(); 
			++it) {
		if (it->option == o) {
			return true;
		}
	}

	return false;
}


string
DBDriver::query_info()
{
	const char* i = mysql_info(&mysql_);
	return i ? string(i) : string();
}


bool
DBDriver::set_option(int o, bool arg)
{
	// If we get through this loop and n is 1, only one bit is set in
	// the option value, which is as it should be.
	int n = o;
	while (n && ((n & 1) == 0)) {
		n >>= 1;
	}
	
	if ((n == 1) && 
			(o >= CLIENT_LONG_PASSWORD) && 
			(o <= CLIENT_MULTI_RESULTS)) {
		// Option value seems sane, so go ahead and set/clear the flag
		if (arg) {
			mysql_.client_flag |= o;
		}
		else {
			mysql_.client_flag &= ~o;
		}

		return true;
	}
	else {
		// Option value is outside the range we understand, or caller
		// erroneously passed a value with multiple bits set.
		return false;
	}
}


std::string
DBDriver::set_option(option::Type o)
{
	if (connected()) {
		// None of the argument-less options can be set once the
		// connection is up.
		return error_message(o, option::err_conn);
	}
	
	bool success = false;
	switch (o) {
		case option::compress:
			success = set_option(MYSQL_OPT_COMPRESS);
			break;

		case option::named_pipe:
			success = set_option(MYSQL_OPT_NAMED_PIPE);
			break;

#if MYSQL_VERSION_ID >= 40101
		case option::use_result:
			success = set_option(MYSQL_OPT_USE_RESULT);
			break;

		case option::use_remote_connection:
			success = set_option(MYSQL_OPT_USE_REMOTE_CONNECTION);
			break;

		case option::use_embedded_connection:
			success = set_option(MYSQL_OPT_USE_EMBEDDED_CONNECTION);
			break;

		case option::guess_connection:
			success = set_option(MYSQL_OPT_GUESS_CONNECTION);
			break;
#endif
		default:
			return error_message(o, option::err_type);
	}

	if (success) {
		applied_options_.push_back(option::Info(o));
		return "";
	}
	else {
		return error_message(o, option::err_value);
	}
}


std::string
DBDriver::set_option(option::Type o, const char* arg)
{
	if (connected()) {
		// None of the options taking a char* argument can be set once
		// the connection is up.
		return error_message(o, option::err_conn);
	}

	bool success = false;
	switch (o) {
		case option::init_command:
			success = set_option(MYSQL_INIT_COMMAND, arg);
			break;

		case option::read_default_file:
			success = set_option(MYSQL_READ_DEFAULT_FILE, arg);
			break;

		case option::read_default_group:
			success = set_option(MYSQL_READ_DEFAULT_GROUP, arg);
			break;

		case option::set_charset_dir:
			success = set_option(MYSQL_SET_CHARSET_DIR, arg);
			break;

		case option::set_charset_name:
			success = set_option(MYSQL_SET_CHARSET_NAME, arg);
			break;

#if MYSQL_VERSION_ID >= 40100
		case option::shared_memory_base_name:
			success = set_option(MYSQL_SHARED_MEMORY_BASE_NAME, arg);
			break;
#endif
#if MYSQL_VERSION_ID >= 40101
		case option::set_client_ip:
			success = set_option(MYSQL_SET_CLIENT_IP, arg);
			break;
#endif
		default:
			return error_message(o, option::err_type);
	}

	if (success) {
		applied_options_.push_back(option::Info(o, arg));
		return "";
	}
	else {
		return error_message(o, option::err_value);
	}
}


std::string
DBDriver::set_option(option::Type o, unsigned int arg)
{
	if (connected()) {
		// None of the options taking an int argument can be set once
		// the connection is up.
		return error_message(o, option::err_conn);
	}

	bool success = false;
	switch (o) {
		case option::connect_timeout:
			success = set_option(MYSQL_OPT_CONNECT_TIMEOUT, &arg);
			break;

		case option::local_infile:
			success = set_option(MYSQL_OPT_LOCAL_INFILE, &arg);
			break;

#if MYSQL_VERSION_ID >= 40100
		case option::protocol:
			success = set_option(MYSQL_OPT_PROTOCOL, &arg);
			break;
#endif
#if MYSQL_VERSION_ID >= 40101
		case option::read_timeout:
			success = set_option(MYSQL_OPT_READ_TIMEOUT, &arg);
			break;

		case option::write_timeout:
			success = set_option(MYSQL_OPT_WRITE_TIMEOUT, &arg);
			break;
#endif
		default:
			return error_message(o, option::err_type);
			return false;
	}

	if (success) {
		applied_options_.push_back(option::Info(o, arg));
		return "";
	}
	else {
		return error_message(o, option::err_value);
	}
}


std::string
DBDriver::set_option(option::Type o, bool arg)
{
	if (connected() &&
			(o != option::multi_results) &&
			(o != option::multi_statements)) {
		// We're connected and it isn't an o that can be set
		// after connection is up, so complain to user.
		return error_message(o, option::err_conn);
	}

	bool success = false;
	switch (o) {
#if MYSQL_VERSION_ID >= 40101
		case option::secure_auth:
			success = set_option(MYSQL_SECURE_AUTH, &arg);
			break;

		case option::multi_results:
		case option::multi_statements:
			// This connection o is unique in that it can be set
			// either before or after the connection is up.  Note
			// however that setting either of these two options after
			// connection is up sets both; else, it sets only the one.
			if (connected()) {
				success = set_option(
						arg ?
						MYSQL_OPTION_MULTI_STATEMENTS_ON :
						MYSQL_OPTION_MULTI_STATEMENTS_OFF);
			}
			else {
				success = set_option(
						o == option::multi_results ?
						CLIENT_MULTI_RESULTS :
						CLIENT_MULTI_STATEMENTS,
						arg);
			}
			break;
#endif
#if MYSQL_VERSION_ID >= 50003
		case option::report_data_truncation:
			success = set_option(MYSQL_REPORT_DATA_TRUNCATION, &arg);
			break;
#endif
#if MYSQL_VERSION_ID >= 50013
		case option::reconnect:
			success = set_option(MYSQL_OPT_RECONNECT, &arg);
			break;
#endif
		case option::found_rows:
			success = set_option(CLIENT_FOUND_ROWS, arg);
			break;

		case option::ignore_space:
			success = set_option(CLIENT_IGNORE_SPACE, arg);
			break;

		case option::interactive:
			success = set_option(CLIENT_INTERACTIVE, arg);
			break;

		case option::local_files:
			success = set_option(CLIENT_LOCAL_FILES, arg);
			break;

		case option::no_schema:
			success = set_option(CLIENT_NO_SCHEMA, arg);
			break;

		default:
			return error_message(o, option::err_type);
	}

	if (success) {
		applied_options_.push_back(option::Info(o, arg));
		return "";
	}
	else {
		return error_message(o, option::err_value);
	}
}


bool
DBDriver::shutdown()
{
	return mysql_shutdown(&mysql_ SHUTDOWN_ARG);
}


bool
DBDriver::thread_aware() const
{
#if defined(MYSQLPP_PLATFORM_WINDOWS) || defined(HAVE_PTHREAD) || defined(HAVE_SYNCH_H)
	// Okay, good, MySQL++ itself is thread-aware, but only return true
	// if the underlying C API library is also thread-aware.
	return mysql_thread_safe();
#else
	// MySQL++ itself isn't thread-aware, so we don't need to do any
	// further tests.  All pieces must be thread-aware to return true.
	return false;	
#endif
}

} // end namespace mysqlpp

