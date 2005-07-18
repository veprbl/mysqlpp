/// \file lockable.h
/// \brief Declares interface that allows a class to declare itself as
/// "lockable".
///
/// The meaning of a class being lockable is very much per-class
/// specific in this version of MySQL++.  In a future version, it will
/// imply that operations that aren't normally thread-safe will use
/// platform mutexes if MySQL++ is configured to support them.  This is
/// planned for v2.1. (See the ChangeLog for the details.)  If a patch
/// appears before then, it will certainly be applied sooner!  In the
/// meantime, do not depend on this mechanism for thread safety; you
/// will have to serialize access to some resources yourself.

/***********************************************************************
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

#ifndef MYSQLPP_LOCKABLE_H
#define MYSQLPP_LOCKABLE_H

namespace mysqlpp {

/// \brief Interface allowing a class to declare itself as "lockable".
///
/// A class derives from this one to acquire a standard interface for
/// serializing operations that may not be thread-safe.

class Lockable
{
protected:
	/// \brief Default constructor
	Lockable(bool lck = false) :
	locked_(lck)
	{
	}

	/// \brief Destroy object
	virtual ~Lockable() { }

	/// \brief Lock the object
	///
	/// \return true if object was already locked
	virtual bool lock()
	{
		if (locked_) {
			return true;
		}
		locked_ = true;
		return false;
	}

	/// \brief Unlock the object
	virtual void unlock() { locked_ = false; }

	/// \brief Returns true if object is locked
	bool locked() const { return locked_; }

protected:
	/// \brief Set the lock state.  Protected, because this method is
	/// only for use by subclass assignment operators and the like.
	void set_lock(bool b) { locked_ = b; }

private:
	bool locked_;
};

} // end namespace mysqlpp

#endif // MYSQLPP_LOCKABLE_H

