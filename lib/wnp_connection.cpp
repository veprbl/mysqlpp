/***********************************************************************
 wnp_connection.cpp - Implements the WindowsNamedPipeConnection class.

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

#define MYSQLPP_NOT_HEADER
#include "common.h"
#include "wnp_connection.h"

#include "exceptions.h"

using namespace std;

namespace mysqlpp {

static cchar* common_complaint =
		"WindowsNamedPipeConnection only works on Windows";


bool
WindowsNamedPipeConnection::connect(cchar* db, cchar* user, cchar* pass)
{
#if defined(MYSQLPP_PLATFORM_WINDOWS)
	return Connection::connect(db, ".", user, pass);
#else
	error_message_ = common_complaint;
	if (throw_exceptions()) {
		throw ConnectionFailed(error_message_.c_str());
	}
	else {
		return false;
	}
#endif
}


bool
WindowsNamedPipeConnection::is_wnp(cchar* server)
{
#if defined(MYSQLPP_PLATFORM_WINDOWS)
	return server && (strcmp(".") == 0);
#else
	return false;
#endif
}

} // end namespace mysqlpp

