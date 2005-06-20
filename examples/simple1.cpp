/***********************************************************************
 simple1.cpp - Example showing the simplest way to get the contents of
 	a table with MySQL++.

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

#include "util.h"

#include <mysql++.h>

int
main(int argc, char *argv[])
{
	// Connect to the sample database
	mysqlpp::Connection con;
	mysqlpp::NoExceptions ne(con);
	if (!connect_to_db(argc, argv, con)) {
		return 1;
	}

	// Retrieve the entire stock table from the database server
	// we're connected to, and print its contents out.
	mysqlpp::Query query = con.query();
	print_stock_table(query);

	return 0;
}
