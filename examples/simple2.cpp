/***********************************************************************
 simple2.cpp - Example showing how to use the 'use' method of retrieving
	a table, as opposed to the more common 'store' method.

 Copyright (c) 2005 by Educational Technology Resources, Inc.
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

int
main(int argc, char *argv[])
{
	// Connect to the sample database.
	mysqlpp::Connection con(false);
	if (!connect_to_db(argc, argv, con)) {
		return 1;
	}
	cout << "Connected to database successfully!" << endl;

	// Ask for all rows from the sample stock table set up by resetdb.
	// Unlike simple1 example, we don't store result set in memory.
	mysqlpp::Query query = con.query();
	query << "select * from stock";
	mysqlpp::ResUse res = query.use();

	// Retreive result rows one by one, and display them.
	if (res) {
		mysqlpp::Row row;
		while (row = res.fetch_row()) {
			string item(row["item"]);
			cout << item << ", " << row["num"] << " units, weight " <<
					row["weight"] << ", $" << row["price"] <<
					", entered on " << row["sdate"] << endl;
		}
		return 0;
	}
	else {
		cerr << "Failed to get stock item: " << query.error() << endl;
		return 1;
	}
}
