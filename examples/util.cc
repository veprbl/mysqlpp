#include <iostream>
#include <iomanip>
#include "util.hh"

void print_stock_table(Query& query) {
  query.reset();
  (std::ostream&)query << "select * from stock";
  // You can write to the query object like you would any other ostrem

  Result res = query.store();
  // Query::store() executes the query and returns the results

  cout << "Query: " << query.preview() << endl;
  // Query::preview() simply returns a string with the current query
  // string in it.

  cout << "Records Found: " << res.size() << endl << endl;
  
  Row row;
  cout.setf(ios::left);
  cout << std::setw(20) << "Item"
       << std::setw(9)  << "Num"
       << std::setw(9)  << "Weight"
       << std::setw(9)  << "Price"
       << "Date" << endl
       << endl;
  
  Result::iterator i;
  // The Result class has a read-only Random Access Iterator
  for (i = res.begin(); i != res.end(); i++) {
    row = *i; 
    cout << std::setw(20) << row[0].c_str()
	 << std::setw(9)  << row[1].c_str()
	 << std::setw(9)  << row.lookup_by_name("weight").c_str()
      // you can use either the index number or column name when
      // retrieving the colume data as demonstrated above.
	 << std::setw(9)  << row[3].c_str()
	 << row[4] << endl;
  }
}
