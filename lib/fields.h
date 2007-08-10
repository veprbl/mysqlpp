/// \file fields.h
/// \brief Declares a class for holding information about a set of
/// fields.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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

#ifndef MYSQLPP_FIELDS_H
#define MYSQLPP_FIELDS_H

#include "subiter.h"

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT ResUse;
#endif

/// \brief A container similar to \c std::vector for holding
/// mysqlpp::Field records.

class MYSQLPP_EXPORT Fields
{
public:
	typedef int difference_type;			///< type for index differences
	typedef unsigned int size_type;			///< type of returned sizes

	typedef Field value_type;				///< type of data in container
	typedef value_type& reference;			///< reference to value_type
	typedef const value_type& const_reference;///< const ref to value_type
	typedef value_type* pointer;			///< pointer to value_type
	typedef const value_type* const_pointer;///< const pointer to value_type

	/// \brief regular iterator type
	///
	/// Note that this is the same as const_iterator; we don't have a
	/// mutable iterator type.
	typedef subscript_iterator<const Fields, const value_type, size_type,
			difference_type> iterator;	
	typedef iterator const_iterator;		///< constant iterator type

	/// \brief mutable reverse iterator type
	typedef const std::reverse_iterator<iterator> reverse_iterator;			

	/// \brief const reverse iterator type
	typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;		

	/// \brief Return maximum number of elements that can be stored
	/// in container without resizing.
	size_type max_size() const { return size(); }

	/// \brief Returns true if container is empty
	bool empty() const { return size() == 0; }

	/// \brief Return iterator pointing to first element in the
	/// container
	iterator begin() const { return iterator(this, 0); }

	/// \brief Return iterator pointing to one past the last element
	/// in the container
	iterator end() const { return iterator(this, size()); }

	/// \brief Return reverse iterator pointing to first element in the
	/// container
	reverse_iterator rbegin() const { return reverse_iterator(end()); }

	/// \brief Return reverse iterator pointing to one past the last
	/// element in the container
	reverse_iterator rend() const { return reverse_iterator(begin()); }

	/// \brief Default constructor
	Fields() { }
	
	/// \brief Create a field list from a result set
	Fields(ResUse* r) :
	res_(r)
	{
	}

	/// \brief Returns a field given its index.
	const value_type& at(int i) const;

	/// \brief Returns a field given its index.
	///
	/// Just a synonym for at()
	const value_type& operator [](int i) const { return at(i); }

	size_type size() const;	///< get the number of fields

private:
	mutable ResUse* res_;
};

} // end namespace mysqlpp

#endif
