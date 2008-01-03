/***********************************************************************
 custom1.cpp - Example that produces the same results as simple1, but it
 	uses a Specialized SQL Structure to store the results instead of a
	MySQL++ Result object.
 
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

#include "cmdline.h"
#include "printdata.h"
#include "stock.h"

#include <iostream>
#include <vector>

using namespace std;

int
main(int argc, char *argv[])
{
#if defined(MYSQLPP_SSQLS_COMPATIBLE)
	// Get database access parameters from command line
    const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass)) {
		return 1;
	}

	try {						
		// Establish the connection to the database server.
		mysqlpp::Connection con(db, server, user, pass);

		// Retrieve just the item column from the stock table, and store
		// the data in a vector of 'stock' SSQLS structures.  See the
		// user manual for the consequences arising from this quiet
		// ability to store a subset of the table in the stock SSQLS.
		mysqlpp::Query query = con.query("select item from stock");
		vector<stock> res;
		query.storein(res);

		// Display the items
		cout << "We have:" << endl;
		vector<stock>::iterator it;
		for (it = res.begin(); it != res.end(); ++it) {
			cout << '\t' << it->item << endl;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions; e.g. type mismatch populating 'stock'
		cerr << "Conversion error: " << er.what() << endl <<
				"\tretrieved data size: " << er.retrieved <<
				", actual size: " << er.actual_size << endl;
		return -1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
#else
	// MySQL++ works under Visual C++ 2003 with only one excpetion,
	// SSQLS, so we have to stub out the examples to avoid build errors.
	cout << argv[0] << " requires Visual C++ 2005 or newer." << endl;
#endif

	return 0;
}
