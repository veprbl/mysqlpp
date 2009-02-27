/// \file insertpolicy.h
/// \brief Declares the InsertPolicy classes.
///
/// These objects are used by the Query::insertfrom() method to insert
/// collections of SSQLS objects when certain constraints in the raw
/// SQL command, such as a maximum are required.
///
/// This file is not meant to be included in end-user code.  It's
/// included in Query's public interface, since it is only used with
/// Query::insertfrom().  You access it as Query::InsertPolicy<T>

/***********************************************************************
 Copyright (c) 2008 by AboveNet, Inc.  Others may also hold copyrights 
 on code in this file.  See the CREDITS file in the top directory of 
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

#if !defined(MYSQLPP_INSERTPOLICY_H)
#define MYSQLPP_INSERTPOLICY_H

/// \brief The default policy object for doing bulk inserts.
///
/// InsertPolicy objects work hand-in-hand with Query::insertfrom().
///
/// The InsertPolicy object determines if another object can be added
/// to the INSERT statement using either internal state variables or
/// by looking at the length of the query, which is one of the
/// arguments passed in.
///
/// This policy simply creates a single SQL statement regardless
/// of how big it gets.
///
/// These templates use a class called the AccessController, which
/// is a stand-in for the mysqlpp::Transaction class and defaults to 
/// the Transaction class.  Since some of the InsertPolicy objects
/// (SizeThresholdInsertPolicy and MaxPacketInsertPolicy) may cause 
/// Query::insertfrom() to issue multiple SQL statements to perform
/// the insertion of all the objects in the container, and a failure in
/// one of these statements would leave the table in an indeterminate
/// state, the whole operation is wrapped in a transaction.
///
/// However, a transaction may not be desired if the calling code
/// is managing transactions, or transactions are not being used for 
/// some other reason.  In this case, the template can be used
/// with the provided NoTransaction object defined above.  That object
/// matches the Transaction object's interface but does nothing.
///
/// The decision to use the InsertPolicy object was made in order to 
/// allow the possibility of a user-created object that uses some 
/// other criterion to determine how many objects to include in a 
/// single INSERT statement.
template <class AccessController = Transaction>
class MYSQLPP_EXPORT DefaultInsertPolicy
{
public:
	/// \brief Constructor
	DefaultInsertPolicy() { }

	/// \brief Destructor
	~DefaultInsertPolicy() { }

	/// \brief Can we add another object to the query?
	///
	/// \param size current length of the INSERT statement
	/// \param object the SSQLS object to be added
	///
	/// \retval true if the object is allowed to be added to the
	/// INSERT statement
	template <class RowT>
	bool can_add(int size, const RowT& object) const { return true; }

	/// \brief Alias for our access controller type
	typedef AccessController access_controller;
};


/// \brief A policy object that triggers a new INSERT statement
/// after a size threshold for the length of the INSERT statement
/// is exceeded.
///
/// Although this insert policy isn't completely deterministic, it
/// avoids building the VALUES expression for the SSQLS object
/// passed in.
template <class AccessController = Transaction>
class MYSQLPP_EXPORT SizeThresholdInsertPolicy
{
public:
	/// \brief Constructor
	SizeThresholdInsertPolicy(int size) :
	size_(size)
	{
	}

	/// \brief Destructor
	~SizeThresholdInsertPolicy() { }

	/// \brief Can we add another object to the query?
	///
	/// \param size current length of the INSERT statement
	/// \param object the SSQLS object to be added
	///
	/// \retval true if the object is allowed to be added to the
	/// INSERT statement
	template <class RowT>
	bool can_add(int size, const RowT& object) const {
		return (size < size_);
	}

	/// \brief Alias for our access controller type
	typedef AccessController access_controller;

private:
	int size_;
};


/// \brief A policy object that triggers a new INSERT statement
/// if the object to be added would cause the statement to exceed
/// a maximum size.
///
/// This differs from the SizeThresholdInsertPolicy in that it builds
/// the VALUES expression and checks whether it would cause the
/// length of the INSERT statement to exceed the maximum size.
template <class AccessController = Transaction>
class MYSQLPP_EXPORT MaxPacketInsertPolicy
{
public:
	/// \brief Constructor
	///
	/// \param con connection object used for escaping text
	/// \param size the maximum allowed size for an INSERT
	///     statement
	MaxPacketInsertPolicy(Connection* con, int size) :
	conn_(con), size_(size)
	{
	}

	/// \brief Constructor
	///
	/// This version does not use a Connection* so it will not be
	/// able to take the character set into account when escaping
	/// the text.
	///
	/// \param size the maximum allowed size for an INSERT
	///     statement
	MaxPacketInsertPolicy(int size) :
	conn_(0), size_(size)
	{
	}

	/// \brief Destructor
	~MaxPacketInsertPolicy() { }

	/// \brief Can we add another object to the query?
	///
	/// \param size current length of the INSERT statement
	/// \param object the SSQLS object to be added
	///
	/// \retval true if the object is allowed to be added to the
	///     INSERT statement
	template <class RowT>
	bool can_add(int size, const RowT& object) const {
		if (size < size_) {
			// Haven't hit size threshold yet, so see if this next
			// item pushes it over the line.
			SQLStream s(conn_);
			s << ",(" << object.value_list() << ")";
			return (size_ - size) >= (int)s.str().size();
		}
		else {
			// Already too much in query buffer!
			return false;
		}
	}

	/// \brief Alias for our access controller type
	typedef AccessController access_controller;

private:
	Connection* conn_;
	int size_;
};

#endif // !defined(MYSQLPP_INSERTPOLICY_H)

