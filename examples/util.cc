#include "util.h"

#include <iostream>
#include <iomanip>

using namespace std;

const char* kpcSampleDatabase = "mysql_cpp_data";

void
print_stock_table(mysqlpp::Query& query)
{
	query.reset();
	query << "select * from stock";
	// You can write to the query object like you would any other ostrem

	mysqlpp::Result res = query.store();
	// Query::store() executes the query and returns the results

	cout << "Query: " << query.preview() << endl;
	// Query::preview() simply returns a string with the current query
	// string in it.

	cout << "Records Found: " << res.size() << endl << endl;

	mysqlpp::Row row;
	cout.setf(ios::left);
	cout << setw(20) << "Item"
		<< setw(9) << "Num"
		<< setw(9) << "Weight"
		<< setw(9) << "Price" << "Date" << endl << endl;

	mysqlpp::Result::iterator i;
	// The Result class has a read-only Random Access Iterator
	for (i = res.begin(); i != res.end(); i++) {
		row = *i;
		cout << setw(20) << row[0].c_str()
			<< setw(9) << row[1].c_str()
			<< setw(9) << row.lookup_by_name("weight").c_str()
			// you can use either the index number or column name when
			// retrieving the colume data as demonstrated above.
			<< setw(9) << row[3].c_str()
			<< row[4] << endl;
	}
}


bool
connect_sample_db(int argc, char *argv[], mysqlpp::Connection& con,
		const char *kdb)
{
	if (argc < 1) {
		cerr << "Bad argument count: " << argc << '!' << endl;
		return false;
	}

	if ((argc > 1) && (argv[1][0] == '-')) {
		cout << "usage: " << argv[0] << " [host] [user] [password]" <<
				endl;
		cout << endl << "\tConnects to database ";
		if (kdb) {
			cout << '"' << kdb << '"';
		}
		else {
			cout << "server";
		}
		cout << " on localhost using your user" << endl;
		cout << "\tname and no password by default." << endl << endl;
		return false;
	}

	if (!kdb) {
		kdb = kpcSampleDatabase;
	}

	bool success;
	if (argc == 1) {
		success = con.connect(kdb);
	}
	else if (argc == 2) {
		success = con.connect(kdb, argv[1]);
	}
	else if (argc == 3) {
		success = con.connect(kdb, argv[1], argv[2]);
	}
	else if (argc >= 4) {
		success = con.connect(kdb, argv[1], argv[2], argv[3]);
	}

	if (!success) {
		cerr << "Database connection failed." << endl << endl;
	}

	return success;
}

