/// \file np_connection.h
/// \brief Declares the WindowsNamedPipeConnection class.

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

#if !defined(MYSQLPP_WNP_CONNECTION_H)
#define MYSQLPP_WNP_CONNECTION_H

#include "connection.h"

namespace mysqlpp {

/// \brief Specialization of \c Connection for Windows named pipes
///
/// This class just simplifies the connection creation interface of
/// \c Connection.  It does not add new functionality.

class WindowsNamedPipeConnection : public Connection
{
public:
	/// \brief Create object without connecting it to the MySQL server.
	WindowsNamedPipeConnection() :
	Connection()
	{
	}

	/// \brief Create object and connect to database server over Windows
	/// named pipes in one step.
	///
	/// \param db name of database to use
	/// \param user user name to log in under, or 0 to use the user
	///		name the program is running under
	/// \param password password to use when logging in
	WindowsNamedPipeConnection(cchar* db = 0, cchar* user = 0,
			cchar* password = 0)
	{
		connect(db, user, password);
	}

	/// \brief Establish a new connection using the same parameters as
	/// an existing connection.
	///
	/// \param other pre-existing connection to clone
	WindowsNamedPipeConnection(const WindowsNamedPipeConnection& other)
	{
		copy(other);	// slices it, but that's okay
	}

	/// \brief Destroy object
	~WindowsNamedPipeConnection() { }

	/// \brief Connect to database after object is created.
	///
	/// It's better to use the connect-on-create constructor if you can.
	/// See its documentation for the meaning of these parameters.
	///
	/// If you call this method on an object that is already connected
	/// to a database server, the previous connection is dropped and a
	/// new connection is established.
	bool connect(cchar* db = 0, cchar* user = 0, cchar* password = 0);

	/// \brief Check that given string denotes a Windows named pipe
	/// connection to MySQL
	///
	/// \param server the server address
	///
	/// \return false if server address does not denote a Windows
	/// named pipe connection, or we are not running on Windows
	static bool is_wnp(cchar* server);
};


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_WNP_CONNECTION_H)

