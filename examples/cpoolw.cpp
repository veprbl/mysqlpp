/***********************************************************************
 cpoolw.cpp - Windows threads version of ConnectionPool example.  Shows
 	how to create and use a concrete ConnectionPool derivative.

 Copyright (c) 2008 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS file in the top directory of the distribution for details.

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

#include <mysql++.h>

#include <iostream>

using namespace std;


// Define a concrete ConnectionPool derivative.  Takes connection
// parameters as inputs to its ctor, which it uses to create the
// connections we're called upon to make.  Note that we also declare
// a global pointer to an object of this type, which we create soon
// after startup; this should be a common usage pattern, as what use
// are multiple pools?
class SimpleConnectionPool : public mysqlpp::ConnectionPool
{
public:
	// The object's only constructor
	SimpleConnectionPool(const char* db, const char* server,
			const char* user, const char* password) :
	db_(db ? db : ""),
	server_(server ? server : ""),
	user_(user ? user : ""),
	password_(password ? password : "")
	{
	}

	// The destructor.  We _must_ call ConnectionPool::clear() here,
	// because our superclass can't do it for us.
	~SimpleConnectionPool()
	{
		clear();
	}

protected:
	// Superclass overrides
	mysqlpp::Connection* create()
	{
		// Create connection using the parameters we were passed upon
		// creation.  This could be something much more complex, but for
		// the purposes of the example, this suffices.
		cout << endl << "Creating new pooled connection!" << endl;
		return new mysqlpp::Connection(
				db_.empty() ? 0 : db_.c_str(),
				server_.empty() ? 0 : server_.c_str(),
				user_.empty() ? 0 : user_.c_str(),
				password_.empty() ? "" : password_.c_str());
	}

	void destroy(mysqlpp::Connection* cp)
	{
		// Our superclass can't know how we created the Connection, so
		// it delegates destruction to us, to be safe.
		cout << endl << "Destroying pooled connection!" << endl;
		delete cp;
	}

	unsigned int max_idle_time()
	{
		// Set our idle time at an example-friendly 3 seconds.  A real
		// pool would return some fraction of the server's connection
		// idle timeout instead.
		return 3;
	}

private:
	// Our connection parameters
	std::string db_, server_, user_, password_;
};
SimpleConnectionPool* poolptr = 0;


static DWORD WINAPI
worker_thread(LPVOID running_flag)
{
	// Pull data from the sample table a bunch of times, releasing the
	// connection we use each time.
	for (int i = 0; i < 6; ++i) {
		// Go get a free connection from the pool, or create a new one
		// if there are no free conns yet.
		mysqlpp::Connection* cp = poolptr->grab();
		if (!cp) {
			cerr << "Failed to get a connection from the pool!" << endl;
			break;
		}

		// Pull a copy of the sample stock table and print a dot for
		// each row in the result set.
		mysqlpp::Query query(cp->query("select * from stock"));
		mysqlpp::StoreQueryResult res = query.store();
		for (int j = 0; j < res.num_rows(); ++j) {
			cout.put('.');
		}

		// Immediately release the connection once we're done using it.
		// If we don't, the pool can't detect idle connections reliably.
		poolptr->release(cp);

		// Delay 1-4 seconds before doing it again.  Because this can
		// delay longer than the idle timeout, we'll occasionally force
		// the creation of a new connection on the next loop.
		Sleep(1000 * (rand() % 4 + 1));	
	}

	// Tell main() that this thread is no longer running
	*reinterpret_cast<bool*>(running_flag) = false;
	
	return 0;
}


int
main(int argc, char *argv[])
{
	// Get database access parameters from command line
    const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass)) {
		return 1;
	}

	// Create the pool and grab a connection.  We do it mainly to test
	// that the parameters are good before we start doing real work.
	// Note that unlike cpoolp, we don't bother to check that MySQL++
	// and the underlying C API library are thread-aware.  It's not
	// optional for Windows.
	poolptr = new SimpleConnectionPool(db, server, user, pass);
	try {
		mysqlpp::Connection* cp = poolptr->grab();
		poolptr->release(cp);
	}
	catch (mysqlpp::Exception& e) {
		cerr << "Failed to set up initial pooled connection: " <<
				e.what() << endl;
		return 1;
	}

	// Setup complete.  Now let's spin some threads...
	cout << "Pool created and working correctly.  Now to do some "
			"real work..." << endl;
	srand(time(0));
	bool running[] = {
			true, true, true, true, true, true, true,
			true, true, true, true, true, true, true };
	const int num_threads = sizeof(running) / sizeof(running[0]);
	int i;
	for (i = 0; i < num_threads; ++i) {
		if (CreateThread(0, 0, worker_thread, running + i, 0, 0) == 0) {
			cerr << "Failed to create thread " << i <<
					": error code " << GetLastError() << endl;
			return 1;
		}
	}

	// Test the 'running' flags every second until we find that they're
	// all turned off, indicating that all threads are stopped.
	cout << endl << "Waiting for threads to complete..." << endl;
	do {
		Sleep(1000);
		i = 0;
		while (i < num_threads && !running[i]) ++i;
	}
	while (i < num_threads);
	cout << endl << "All threads stopped!" << endl;

	// Shut it all down...
	delete poolptr;

	return 0;
}
