/// \file connection.h
/// \brief Declares the Connection class.
///
/// Every program using MySQL++ must create a Connection object, which
/// manages information about the connection to the MySQL database, and
/// performs connection-related operations once the connection is up.
/// Subordinate classes, such as Query and Row take their defaults as
/// to whether exceptions are thrown when errors are encountered from
/// the Connection object that created them, directly or indirectly.

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

#ifndef MYSQLPP_CONNECTION_H
#define MYSQLPP_CONNECTION_H

#include "platform.h"

#include "defs.h"

#include "lockable.h"
#include "noexceptions.h"

#include <mysql.h>

#include <string>

namespace mysqlpp {

class Query;

/// \brief Manages the connection to the MySQL database.

class Connection : public OptionalExceptions, public Lockable
{
public:
	/// \brief Create object without connecting it to the MySQL server.
	///
	/// \param te if true, exceptions are thrown on errors
	MYSQLPP_EXPORT Connection(bool te = true);

	/// \brief Create object and connect to database server in one step.
	///
	/// This constructor allows you to most fully specify the options
	/// used when connecting to the MySQL database.  It is the thinnest
	/// layer in MySQL++ over the MySQL C API function
	/// \c mysql_real_connect().  The correspondence isn't exact as
	/// we have some additional parameters you'd have to set with
	/// \c mysql_option() when using the C API.
	///
	/// \param db name of database to use
	/// \param host host name or IP address of MySQL server, or 0
	/// 	if server is running on the same host as your program
	/// \param user user name to log in under, or 0 to use the user
	///		name this program is running under
	/// \param passwd password to use when logging in
	/// \param port TCP port number MySQL server is listening on, or 0
	///		to use default value
	/// \param compress if true, compress data passing through
	///		connection, to save bandwidth at the expense of CPU time
	/// \param connect_timeout max seconds to wait for server to
	///		respond to our connection attempt
	/// \param socket_name Unix domain socket server is using, if
	///		connecting to MySQL server on the same host as this program
	///		running on, or 0 to use default name
	///	\param client_flag special connection flags. See MySQL C API
	///		documentation for \c mysql_real_connect() for details.
	MYSQLPP_EXPORT Connection(const char* db, const char* host = "",
			const char* user = "", const char* passwd = "",
			uint port = 0, my_bool compress = 0,
			unsigned int connect_timeout = 60, cchar* socket_name = 0,
			unsigned int client_flag = 0);

	/// \brief Destroy connection object
	MYSQLPP_EXPORT ~Connection();

	/// \brief Connect to database after object is created.
	///
	/// It's better to use the connect-on-create constructor if you can.
	/// See its documentation for the meaning of these parameters.
	///
	/// If you call this method on an object that is already connected
	/// to a database server, the previous connection is dropped and a
	/// new connection is established.
	MYSQLPP_EXPORT bool connect(cchar* db = "", cchar* host = "",
			cchar* user = "", cchar* passwd = "", uint port = 0,
			my_bool compress = 0, unsigned int connect_timeout = 60,
			cchar* socket_name = 0, unsigned int client_flag = 0);

	/// \brief Close connection to MySQL server.
	///
	/// Closes the connection to the MySQL server.
	MYSQLPP_EXPORT void close()
	{
		mysql_close(&mysql_);
		is_connected_ = false;
	}
	
	/// \brief Calls MySQL C API function \c mysql_info() and returns
	/// result as a C++ string.
	MYSQLPP_EXPORT std::string info();

	/// \brief return true if connection was established successfully
	///
	/// \return true if connection was established successfully
	MYSQLPP_EXPORT bool connected() const
	{
		return is_connected_;
	}

	/// \brief Return true if the last query was successful
	MYSQLPP_EXPORT bool success() const
	{
		return success_;
	}

	/// \brief Alias for close()
	MYSQLPP_EXPORT void purge() { close(); }

	/// \brief Return a new query object.
	///
	/// The returned query object is tied to this MySQL connection,
	/// so when you call a method like
	/// \link mysqlpp::Query::execute() execute() \endlink
	/// on that object, the query is sent to the server this object
	/// is connected to.
	MYSQLPP_EXPORT Query query();

	/// \brief Alias for success()
	///
	/// Alias for success() member function. Allows you to have code
	/// constructs like this:
	///
	/// \code
	///	    Connection conn;
	///	    .... use conn
	///	    if (conn) {
	///	        ... last SQL query was successful
	///	    }
	///	    else {
	///	        ... error occurred in SQL query
	///	    }
	/// \endcode
	MYSQLPP_EXPORT operator bool() { return success(); }

	/// \brief Return error message for last MySQL error associated with
	/// this connection.
	///
	/// Simply wraps \c mysql_error() in the C API.
	MYSQLPP_EXPORT const char* error()
	{
		return mysql_error(&mysql_);
	}

	/// \brief Return last MySQL error number associated with this
	/// connection
	///
	/// Simply wraps \c mysql_errno() in the C API.
	MYSQLPP_EXPORT int errnum() { return mysql_errno(&mysql_); }

	/// \brief Wraps MySQL C API function \c mysql_refresh()
	///
	/// The corresponding C API function is undocumented.  All I know
	/// is that it's used by \c mysqldump and \c mysqladmin, according
	/// to MySQL bug database entry http://bugs.mysql.com/bug.php?id=9816
	/// If that entry changes to say that the function is now documented,
	/// reevaluate whether we need to wrap it.  It may be that it's not
	/// supposed to be used by regular end-user programs.
	MYSQLPP_EXPORT int refresh(unsigned int refresh_options)
	{
		return mysql_refresh(&mysql_, refresh_options);
	}

	/// \brief "Pings" the MySQL database
	///
	/// If server doesn't respond, this function tries to reconnect.
	/// 
	/// \retval 0 if server is responding, regardless of whether we had
	/// to reconnect or not
	/// \retval nonzero if server did not respond to ping and we could
	/// not re-establish the connection
	///
	/// Simply wraps \c mysql_ping() in the C API.
	MYSQLPP_EXPORT int ping() { return mysql_ping(&mysql_); }

	/// \brief Kill a MySQL server thread
	///
	/// \param pid ID of thread to kill
	///
	/// Simply wraps \c mysql_kill() in the C API.
	MYSQLPP_EXPORT int kill(unsigned long pid)
	{
		return mysql_kill(&mysql_, pid);
	}

	/// \brief Get MySQL client library version
	///
	/// Simply wraps \c mysql_get_client_info() in the C API.
	MYSQLPP_EXPORT std::string client_info()
	{
		return std::string(mysql_get_client_info());
	}

	/// \brief Get information about the network connection
	///
	/// String contains info about type of connection and the server
	/// hostname.
	///
	/// Simply wraps \c mysql_get_host_info() in the C API.
	MYSQLPP_EXPORT std::string host_info()
	{
		return std::string(mysql_get_host_info(&mysql_));
	}

	/// \brief Returns version number of MySQL protocol this connection
	/// is using
	///
	/// Simply wraps \c mysql_get_proto_info() in the C API.
	MYSQLPP_EXPORT int proto_info() 
	{
		return mysql_get_proto_info(&mysql_);
	}

	/// \brief Get the MySQL server's version number
	///
	/// Simply wraps \c mysql_get_server_info() in the C API.
	MYSQLPP_EXPORT std::string server_info()
	{
		return std::string(mysql_get_server_info(&mysql_));
	}

	/// \brief Returns information about MySQL server status
	///
	/// String is similar to that returned by the \c mysqladmin
	/// \c status command.  Among other things, it contains uptime 
	/// in seconds, and the number of running threads, questions
	/// and open tables.
	MYSQLPP_EXPORT std::string stat()
	{
		return std::string(mysql_stat(&mysql_));
	}

	/// \brief Create a database
	///
	/// \param db name of database to create
	///
	/// \return true if database was created successfully
	MYSQLPP_EXPORT bool create_db(const std::string& db);

	/// \brief Drop a database
	///
	/// \param db name of database to destroy
	///
	/// \return true if database was created successfully
	MYSQLPP_EXPORT bool drop_db(const std::string& db);

	/// \brief Change to a different database
	MYSQLPP_EXPORT bool select_db(const std::string& db)
	{
		return select_db(db.c_str());
	}

	/// \brief Change to a different database
	MYSQLPP_EXPORT bool select_db(const char* db);

	/// \brief Ask MySQL server to reload the grant tables
	/// 
	/// User must have the "reload" privilege.
	///
	/// Simply wraps \c mysql_reload() in the C API.  Since that
	/// function is deprecated, this one is, too.  The MySQL++
	/// replacement is execute("FLUSH PRIVILEGES").
	MYSQLPP_EXPORT bool reload();
	
	/// \brief Ask MySQL server to shut down.
	///
	/// User must have the "shutdown" privilege.
	///
	/// Simply wraps \c mysql_shutdown() in the C API.
	MYSQLPP_EXPORT bool shutdown();

	/// \brief Return the connection options object
	MYSQLPP_EXPORT st_mysql_options get_options() const
	{
		return mysql_.options;
	}

	/// \brief Sets the given MySQL server option
	///
	/// Wraps \c mysql_set_server_option() in the C API, except that
	/// it returns true for success, instead of 0.  Because that
	/// function only exists in MySQL v4.1 and higher, this function
	/// always returns false when built against older APIs.
	MYSQLPP_EXPORT bool set_option(int option)
	{
#		if MYSQL_VERSION_ID > 41000
			return !mysql_set_server_option(&mysql_, option);
#		else
			return false;
#		endif
	}

	/// \brief Sets the given MySQL connection option
	///
	/// Wraps \c mysql_option() in the C API, except that it returns
	/// true for success, instead of 0.
	MYSQLPP_EXPORT bool set_option(int option, const char* arg)
	{
		return !mysql_options(&mysql_, mysql_option(option), arg);
	}

	/// \brief Return the number of rows affected by the last query
	///
	/// Simply wraps \c mysql_affected_rows() in the C API.
	MYSQLPP_EXPORT my_ulonglong affected_rows()
	{
		return mysql_affected_rows(&mysql_);
	}

	/// \brief Get ID generated for an AUTO_INCREMENT column in the
	/// previous INSERT query.
	///
	/// \retval 0 if the previous query did not generate an ID.  Use
	/// the SQL function LAST_INSERT_ID() if you need the last ID
	/// generated by any query, not just the previous one.
	MYSQLPP_EXPORT my_ulonglong insert_id()
	{
		return mysql_insert_id(&mysql_);
	}

protected:
	/// \brief Drop the connection to the database server
	///
	/// This method is protected because it should only be used within
	/// the library.  Unless you use the default constructor, this
	/// object should always be connected.
	MYSQLPP_EXPORT void disconnect();

private:
	friend class ResNSel;
	friend class ResUse;
	friend class Query;

	MYSQL mysql_;
	bool is_connected_;
	bool success_;
};


} // end namespace mysqlpp

#endif

