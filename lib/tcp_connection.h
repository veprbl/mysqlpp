/// \file tcp_connection.h
/// \brief Declares the TCPConnection class.

/***********************************************************************
 Copyright (c) 2007 by Educational Technology Resources, Inc.
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

#if !defined(MYSQLPP_TCP_CONNECTION_H)
#define MYSQLPP_TCP_CONNECTION_H

#include "connection.h"

namespace mysqlpp {

/// \brief Specialization of \c Connection for TCP/IP
///
/// This class just simplifies the connection creation interface of
/// \c Connection.  It does not add new functionality.

class TCPConnection : public Connection
{
public:
	/// \brief Create object without connecting it to the MySQL server.
	TCPConnection() :
	Connection()
	{
	}

	/// \brief Create object and connect to database server over TCP/IP
	/// in one step.
	///
	/// \param addr TCP/IP address of server, in either dotted quad form
	///     or as a host or domain name; may be followed by a colon and
	///     a port number or service name to override default port
	/// \param db name of database to use
	/// \param user user name to log in under, or 0 to use the user
	///		name the program is running under
	/// \param password password to use when logging in
	TCPConnection(cchar* db, cchar* addr = 0, cchar* user = 0,
			cchar* password = 0);

	/// \brief Establish a new connection using the same parameters as
	/// an existing connection.
	///
	/// \param other pre-existing connection to clone
	TCPConnection(const TCPConnection& other);

	/// \brief Destroy object
	~TCPConnection() { }

	/// \brief Connect to database after object is created.
	///
	/// It's better to use the connect-on-create constructor if you can.
	/// See its documentation for the meaning of these parameters.
	///
	/// If you call this method on an object that is already connected
	/// to a database server, the previous connection is dropped and a
	/// new connection is established.
	bool connect(cchar* db = 0, cchar* addr = 0, cchar* user = 0,
			cchar* password = 0);

	/// \brief Break the given TCP/IP address up into a separate address
	/// and port form
	///
	/// Does some sanity checking on the address.  Only intended to
	/// try and prevent library misuse, not ensure that the address can
	/// actually be used to contact a server.
	///
	/// It understands the following forms:
	///
	///	- 1.2.3.4
	/// - 1.2.3.4:567
	/// - 1.2.3.4:mysvcport
	/// - a.b.com:89
	/// - a.b.com:mysql
	///
	/// It also understands IPv6 addresses, but to avoid confusion
	/// between the colons they use and the colon separating the address
	/// part from the service/port part, they must be in RFC 2732 form.
	/// Example: \c [2010:836B:4179::836B:4179]:1234
	///
	/// \param addr the address and optional port/service combo to check
	/// on input, and the verified address on successful return
	/// \param port the port number (resolved from the service name if
	/// necessary) on successful return
	/// \param error on false return, reason for failure is placed here
	///
	/// \return false if address fails to pass sanity checks
	static bool parse_address(std::string& addr, unsigned int& port,
			std::string& error);
};


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_TCP_CONNECTION_H)

