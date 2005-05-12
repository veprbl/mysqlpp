/***********************************************************************
 custom1.cpp - Example very similar to simple1, except that it accesses
 	the query results through a Specialized SQL Structure instead of
	through Row objects.
 
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
#include <custom.h>

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace mysqlpp;

// The following is calling a very complex macro which will create
// "struct stock", which has the member variables:
//   string item
//   ...
//   Date sdate
// plus methods to help populate the class from a MySQL row
// among other things that I'll get too in a later example.
sql_create_5(stock,
			 1, 5,				// explained in the user manual
			 string, item,
			 longlong, num,
			 double, weight,
			 double, price,
			 Date, sdate)

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

		vector < stock > res;
		query.storein(res);
		// this is storing the results into a vector of the custom struct
		// "stock" which was created my the macro above.

		cout.setf(ios::left);
		cout << setw(17) << "Item"
			<< setw(4) << "Num"
			<< setw(7) << "Weight"
			<< setw(7) << "Price" << "Date" << endl << endl;

		// Now we we iterate through the vector using an iterator and
		// produce output similar to that using Row
		// Notice how we call the actual variables in i and not an index
		// offset.  This is because the macro at the begging of the file
		// set up an *actual* struct of type stock which contains the 
		// variables item, num, weight, price, and data.

		cout.precision(3);
		vector<stock>::iterator i;
		for (i = res.begin(); i != res.end(); i++) {
			cout << setw(17) << i->item.c_str()
				// unfortunally the gnu string class does not respond to format
				// modifers so I have to convert it to a conat char *.
				<< setw(4) << i->num
				<< setw(7) << i->weight
				<< setw(7) << i->price << i->sdate << endl;
		}
		return 0;

	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
	catch (BadConversion& er) {
		// handle bad conversions
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
		return -1;
	}
	catch (exception & er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
}
