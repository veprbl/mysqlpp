/// \file cpool.h
/// \brief Declares the ConnectionPool class.

/***********************************************************************
 Copyright (c) 2007 by Educational Technology Resources, Inc.
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

#if !defined(MYSQLPP_CPOOL_H)
#define MYSQLPP_CPOOL_H

#include "beemutex.h"

#include <list>

#include <time.h>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Connection;
#endif

/// \brief A class to manage a pool of connections, for use in a
/// multi-threaded program to allow multiple simultaneous queries.
///
/// This class manages a pool of active database connections.  This is
/// useful in a multi-threaded program, because the MySQL C API doesn't
/// permit multiple simultaneous operations on a single connection.  You
/// can get around this limitation by having multiple connections, but
/// this isn't easy to do well.  This class does it well. :)
///
/// The pool use policy is to always use the most recently used
/// connection that's not being used right now.  This ensures that
/// excess connections get killed off reasonably quickly.  By contrast,
/// picking the least recently used connection can result in a large
/// pool of sparsely used connections because we'd keep resetting the
/// last-used time of the least recently used connection.

class ConnectionPool
{
public:
	/// \brief Default ctor
	ConnectionPool() { }

	/// \brief Dtor
	virtual ~ConnectionPool() { }
	
	/// \brief Get the most recently used free connection in the pool.
	///
	/// This method creates a new connection if an unused one doesn't
	/// exist, and destroys any that have remained unused for too long.
	///
	/// \retval a pointer to the most recently used connection
	Connection* grab();

	/// \brief Mark the given connection as no longer in use.
	///
	/// If you don't call this function, connections returned by
	/// connection() will never go away!
	void release(const Connection* pc);

protected:
	//// Subclass overrides
	/// \brief Create a new connection
	///
	/// Subclasses must override this.
	///
	/// Essentially, this method lets your code tell ConnectionPool
	/// what server to connect to, what login parameters to use, what
	/// connection options to enable, etc.  ConnectionPool can't know
	/// any of this without your help.
	///
	/// \retval A connected Connection object
	virtual Connection* create() = 0;

	/// \brief Returns the maximum number of seconds a connection is
	/// able to remain idle before it is dropped.
	///
	/// Subclasses must override this as it encodes a policy issue,
	/// something that MySQL++ can't declare by fiat.
	///
	/// \retval number of seconds before an idle connection is destroyed
	/// due to lack of use
	virtual unsigned int max_idle_time() = 0;

private:
	//// Internal types
	struct ConnectionInfo {
		Connection* conn;
		time_t last_used;
		bool in_use;

		ConnectionInfo(Connection* c) :
		conn(c),
		last_used(time(0)),
		in_use(true)
		{
		}
	};
	typedef std::list<ConnectionInfo> PoolT;
	typedef PoolT::iterator PoolIt;

	//// Internal data
	PoolT pool_;
	BeecryptMutex mutex_;
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_CPOOL_H)

