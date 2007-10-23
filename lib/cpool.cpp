/***********************************************************************
 cpool.cpp - Implements the ConnectionPool class.

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

#include "cpool.h"

#include "connection.h"

namespace mysqlpp {


//// grab //////////////////////////////////////////////////////////////

Connection*
ConnectionPool::grab()
{
	mutex_.lock();

	// Find first unused connection in the pool
	PoolIt it = pool_.begin();
	while ((it != pool_.end()) && it->in_use) {
		++it;
	}
	
	if (it != pool_.end()) {
		// Found at least one unused connection.  Try to find more, and
		// decide which is the most recently used.
		unsigned int tmax = max_idle_time();
		time_t now = time(0);
		PoolIt mru = it;
		++it;
		while (it != pool_.end()) {
			if (it->in_use) {
				// Can't use this one, it's busy
				++it;			
			}
			else if ((now - it->last_used) > tmax) {
				// This one's too old; nuke it
				PoolIt doomed = it;
				++it;
				pool_.erase(doomed);
			}
			else if (it->last_used > mru->last_used) {
				// Ah, found a free one more recently used; hang onto it
				mru = it;
				++it;
			}
		}

		mru->in_use = true;
		mutex_.unlock();
		return mru->conn;
	}
	else {
		// Pool was empty when this function was called, so create and
		// return a new one.
		pool_.push_back(ConnectionInfo(create()));
		Connection* pc = pool_.back().conn;
		mutex_.unlock();
		return pc;
	}
}


//// release ///////////////////////////////////////////////////////////

void
ConnectionPool::release(const Connection* pc)
{
	mutex_.lock();

	for (PoolIt it = pool_.begin(); it != pool_.end(); ++it) {
		if (it->conn == pc) {
			it->in_use = false;
			break;
		}
	}

	mutex_.unlock();
}

} // end namespace mysqlpp

