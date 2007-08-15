/***********************************************************************
 fieldinf.cpp - Shows how to request information about the fields in a 
    table, such as their SQL and C++-equivalent types.

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

#include "util.h"

#include <iostream>
#include <iomanip>

using namespace std;


int
main(int argc, char *argv[])
{
	try {
		mysqlpp::Connection con;
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		mysqlpp::Query query = con.query("select * from stock");
		mysqlpp::Result res = query.store();

		char widths[] = { 8, 15, 57 };
		cout.setf(ios::left);
		cout << setw(widths[0]) << "Field" <<
				setw(widths[1]) << "SQL Type" <<
				setw(widths[2]) << "Equivalent C++ Type" <<
				endl;
		for (int i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i) {
			cout << string(widths[i] - 1, '=') << ' ';
		}
		cout << endl;
		
		for (size_t i = 0; i < res.names()->size(); i++) {
			// Suppress C++ type name outputs when run under dtest,
			// as they're system-specific.
			const char* cname = dtest_mode ? "n/a" : res.types(i).name();
			cout << setw(widths[0]) << res.names(i).c_str() <<
					setw(widths[1]) << res.types(i).sql_name() <<
					setw(widths[2]) << cname <<
					endl;
		}
		cout << endl;

		// Simple type check
		if (res.types(0) == typeid(string)) {
			cout << "SQL type of 'item' field most closely resembles "
					"the C++ string type." << endl;
		}

		// Tricky type check: the 'if' path shouldn't happen because the
		// num field has the NULL attribute.  We need to dig a little
		// deeper if we want to ignore this in our type checks.
		if (res.types(1) == typeid(mysqlpp::longlong)) {
			cout << "Should not happen! Type check failure." << endl;
		}
		else {
			cout << "SQL type of 'num' field most closely resembles "
					"MySQL++'s longlong type." << endl;
		}
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
	}

	return 0;
}
