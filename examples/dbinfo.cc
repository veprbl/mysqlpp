#include "util.h"

#include <mysql++.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace mysqlpp;

vector<string> yy;

int
main(int argc, char* argv[])
{
	Connection con(use_exceptions);
	try {
		connect_to_db(argc, argv, con, "");

		cout << con.client_info() << endl << endl;
		Query query = con.query();

		query << "show databases";
		cout << "Query: " << query.preview() << endl;

		Result res = query.store();
		cout << "Records Found: " << res.size() << endl << endl;

		Row row;
		cout.setf(ios::left);
		cout << setw(17) << "Databases" << endl << endl;

		Result::iterator i;
		for (i = res.begin(); i != res.end(); ++i) {
			row = *i;
			cout << setw(17) << row[0] << endl;
		}
		char database[] = "mysql";
		con.select_db(database);

		query << "show tables";
		cout << "Query: " << query.preview() << endl;

		res = query.store();
		cout << "Records Found: " << res.size() << endl << endl;

		cout.setf(ios::left);
		cout << setw(17) << "Tables" << endl << endl;

		for (i = res.begin(); i != res.end(); ++i) {
			row = *i;
			string xx(row[0]);
			cout << setw(17) << row[0] << endl;
			yy.insert(yy.end(), xx);
		}

		for (unsigned int j = 0; j < yy.size(); ++j) {
			query << "describe " << yy[j] << "";
			cout << query.preview() << endl << endl;
			res = query.store();
			unsigned int columns = res.num_fields(), counter;
			cout << setw(15) << endl;
			for (counter = 0; counter < columns; counter++) {
				cout << setw(15) << res.names(counter) << "  ";
			}
			cout << endl << endl;
			for (i = res.begin(); i != res.end(); ++i) {
				row = *i;
				for (counter = 0; counter < columns; counter++) {
					cout << row[counter] << "  ";
				}
				cout << endl;
			}
		}

		query << "select * from user";
		cout << query.preview() << endl << endl;

		res = query.store();
		int columns = res.num_fields();
		cout << "fields = " << res.num_fields() << "rows = " << res.
			size() << endl;
		volatile MYSQL_RES *ress = res.mysql_result();
		if (!ress)
			return -1;
		for (i = res.begin(); i != res.end(); ++i) {
			row = *i;
			for (int counter = 0; counter < columns; counter++) {
				cout << row[counter] << "  ";
			}
			cout << endl;
		}
	}
	catch (BadQuery& er) {
		// handle any connection or query errors that may come up
#ifdef USE_STANDARD_EXCEPTION
		cerr << "Error: " << er.what() << " " << con.errnum() << endl;
#else
		cerr << "Error: " << er.error << " " << con.errnum() << endl;
#endif
		return -1;
	}
	catch (BadConversion& er) {
		// Handle bad conversions.  We still need to catch bad
		// conversions in case something goes wrong when the data
		// is converted into stock.
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
	catch (exception& er) {
		cerr << "Error: " << er.what() << endl;
		return -1;
	}
#endif
}

