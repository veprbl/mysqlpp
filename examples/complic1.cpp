/***********************************************************************
 complic1.cpp - Example similar to simple1, except that it shows the
 	use of MySQL++'s automatic type conversion facilities.

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

#include <iostream>
#include <iomanip>

using namespace std;
using namespace mysqlpp;

int
main(int argc, char *argv[])
{
	try {
		Connection con(use_exceptions);
		if (!connect_to_db(argc, argv, con)) {
			return 1;
		}

		Query query = con.query();

		query << "select * from stock";
		cout << "Query: " << query.preview() << endl;

		Result res = query.store();
		cout << "Records Found: " << res.size() << endl << endl;

		Row row;
		cout.setf(ios::left);
		cout << setw(17) << "Item" <<
				setw(4) << "Num" <<
				setw(7) << "Weight" <<
				setw(7) << "Price" << "Date" << endl << endl;

		Result::iterator i;

		cout.precision(3);
		for (i = res.begin(); i != res.end(); i++) {
			row = *i;
			// Regarding the casts to double, we do this so that we can
			// set the display precision.  ColData has the nice feature
			// that it will convert to any of the basic C++ types.  If
			// there is a problem in the conversion it will throw an
			// exception (caught below).  To test it, try changing the
			// row[2] to row[0] below.
			cout << setw(17) << row.lookup_by_name("ITEM") <<
					setw(4) << row[1] <<
					setw(7) << double(row[2]) <<
					setw(7) << double(row[3]);

			// The ColData is implicitly converted to a date here.
			Date date = row.lookup_by_name("SDATE");
			cout.setf(ios::right);
			cout.fill('0');
			cout << setw(2) << date.month << "-" << setw(2) <<
					date.day << endl;
			cout.fill(' ');
			cout.unsetf(ios::right);
		}

		return 0;
	}
	catch (BadQuery& er) {
		// Handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (BadConversion& er) {
		// Handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
}
