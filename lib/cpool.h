/// \file cpool.h
/// \brief Declares the ConnectionPool class.

/***********************************************************************
 Copyright (c) 2007 by Educational Technology Resources, Inc. and
 (c) 2007 by Jonathan Wakely.  Others may also hold copyrights on
 code in this file.  See the CREDITS file in the top directory of
 the distribution for details.

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

#include <assert.h>
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
	///
	/// If this assertion is raised, it means the derived class isn't
	/// calling clear() in its dtor.
	virtual ~ConnectionPool() { assert(pool_.empty()); }

	/// \brief Drains the pool, freeing all allocated memory.
	///
	/// A derived class must call this in its dtor to avoid leaking all
	/// Connection objects still in existence.  We can't do it up at
	/// this level because this class's dtor can't call our subclass's
	/// destroy() method.
	void clear();

	/// \brief Grab a free connection from the pool.
	///
	/// This method creates a new connection if an unused one doesn't
	/// exist, and destroys any that have remained unused for too long.
	/// If there is more than one free connection, we return the most
	/// recently used one; this allows older connections to die off over
	/// time when the caller's need for connections decreases.
	///
	/// Do not delete the returned pointer.  This object manages the
	/// lifetime of connection objects it creates.
	///
	/// \retval a pointer to the connection
	Connection* grab();

	/// \brief Return a connection to the pool
	///
	/// Marks the connection as no longer in use.  Also resets the
	/// last-used time to the current time, so a call implies that the
	/// connection was used shortly prior.
	///
	/// This means that you must always release connections as soon as
	/// you're done with them.  Don't hold on to idle connections!  If
	/// you delay releasing them, it screws up the "most recently used"
	/// algorithm.  If you never release them, they can never be closed
	/// when idle, so you might as well not be using a pool.
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

	/// \brief Destroy a connection
	///
	/// Subclasses must override this.
	///
	/// This is for destroying the objects returned by create().
	/// Because we can't know what the derived class did to create the
	/// connection we can't reliably know how to destroy it.
	virtual void destroy(Connection*) = 0;

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

		// Strict weak ordering for ConnectionInfo objects.
		// 
		// This ordering defines all in-use connections to be "less
		// than" those not in use.  Within each group, connections
		// less recently touched are less than those more recent.
		bool operator<(const ConnectionInfo& rhs) const
		{
			const ConnectionInfo& lhs = *this;
			return lhs.in_use == rhs.in_use ?
					lhs.last_used < rhs.last_used :
					lhs.in_use;
		}
	};
	typedef std::list<ConnectionInfo> PoolT;
	typedef PoolT::iterator PoolIt;

	//// Internal support functions
	Connection* find_mru();
	void remove_old_connections();

	//// Internal data
	PoolT pool_;
	BeecryptMutex mutex_;
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_CPOOL_H)

