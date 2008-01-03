/***********************************************************************
 cpoolp.cpp - POSIX threads version of ConnectionPool example.  Shows
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
#include "printdata.h"

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
		cout << "Creating new pooled connection..." << endl;
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
		cout << "Destroying pooled connection..." << endl;
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
}
*poolptr = 0;


int
main(int argc, char *argv[])
{
	// Get database access parameters from command line
    const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass)) {
		return 1;
	}

	// Create the pool and grab a connection.  We do it partly to test
	// that the parameters are good before we start doing real work, and
	// partly because we need a Connection object to call thread_aware()
	// on to check that it's okay to start doing that real work.
	poolptr = new SimpleConnectionPool(db, server, user, pass);
	try {
		mysqlpp::Connection* cp = poolptr->grab();
		if (!cp->thread_aware()) {
			cerr << "MySQL++ wasn't built with thread awareness!  " <<
					argv[0] << " can't run without it." << endl;
			return 1;
		}
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

	// Shut it all down...
	delete poolptr;

	return 0;
}
