#include "util.h"

#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;

int
main(int argc, char *argv[])
{
	try {
		mysqlpp::Connection con(mysqlpp::use_exceptions);
		if (!connect_sample_db(argc, argv, con)) {
			return 1;
		}

		// Create a query object that is bound to con.
		mysqlpp::Query query = con.query();

		// You can write to the query object like you would any other
		// ostream.
		query << "select * from stock";

		// Display the query string.
		cout << "Query: " << query.preview() << endl;

		// Execute the query and save the results
		mysqlpp::Result res = query.store();
		cout << "Records Found: " << res.size() << endl << endl;

		// Display the result table header.
		cout.setf(ios::left);
		cout << setw(17) << "Item" <<
				setw(4) << "Num" <<
				setw(7) << "Weight" <<
				setw(7) << "Price" << "Date" << endl << endl;

		// Use the Result class's read-only random access iterator to 
		// step through the results.
		mysqlpp::Row row;
		mysqlpp::Result::iterator i;
		for (i = res.begin(); i != res.end(); i++) {
			row = *i;
			// Note that you can use either the column index or name
			// to retrieve the data.
			cout << setw(17) << row[0].c_str() <<
					setw(4) << row[1].c_str() <<
					setw(7) << row.lookup_by_name("weight").c_str() <<
					setw(7) << row[3].c_str() <<
					row[4] << endl;
		}
	}
	catch (mysqlpp::BadQuery& er) {
		// handle any connection or query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << endl;
#else
		cerr << "Error: " << er.error << endl;
#endif
		return -1;
	}
	catch (mysqlpp::BadConversion& er) {
		// handle bad conversions
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << "\"." << endl
			<< "retrieved data size: " << er.retrieved
			<< " actual data size: " << er.actual_size << endl;
#else
		cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
			<< er.type_name << "\"." << endl;
#endif
		return -1;
	}
#ifdef USE_STANDARD_EXCEPTION
	catch (exception & er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
#endif

	return 0;
}
