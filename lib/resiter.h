/// \file resiter.h
/// \brief Declares templates for adapting existing classes to
/// be iteratable random-access containers.
///
/// The file name seems to tie it to the mysqlpp::Result class, which
/// is so adapted, but these templates are also used to adapt the
/// mysqlpp::Fields and mysqlpp::Row classes.

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

#ifndef MYSQLPP_RESITER_H
#define MYSQLPP_RESITER_H

#include "common.h"

#include <iterator>

namespace mysqlpp {

/// \brief Iterator that can be subscripted.
///
/// This is the type of iterator offered by most of MySQL++'s
/// container-like classes.

template <class OnType, class ReturnType, class SizeType, class DiffType>
class subscript_iterator : public std::iterator<ReturnType, SizeType>
{
public:
	/// \brief Default constructor
	subscript_iterator() { }

	/// \brief Create iterator given the container and a position
	/// within it.
	subscript_iterator(OnType* what, SizeType pos)
	{
		d_ = what;
		i_ = pos;
	}

	/// \brief Return true if given iterator points to the same
	/// container and the same position within the container.
	bool operator ==(const subscript_iterator& j) const
	{
		return (d_ == j.d_ && i_ == j.i_);
	}
	
	/// \brief Return true if given iterator is different from this
	/// one, but points to the same container.
	bool operator !=(const subscript_iterator& j) const
	{
		return (d_ == j.d_ && i_ != j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// less than the given iterator's.
	bool operator <(const subscript_iterator& j) const
	{
		return (d_ == j.d_ && i_ < j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// greater than the given iterator's.
	bool operator >(const subscript_iterator & j) const
	{
		return (d_ == j.d_ && i_ > j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// less than or equal to the given iterator's.
	bool operator <=(const subscript_iterator & j) const
	{
		return (d_ == j.d_ && i_ <= j.i_);
	}
	
	/// \brief Return true if the given iterator points to the same
	/// container as this one, and that this iterator's position is
	/// greater than or equal to the given iterator's.
	bool operator >=(const subscript_iterator & j) const
	{
		return (d_ == j.d_ && i_ >= j.i_);
	}
	
	/// \brief Dereference the iterator, returning a copy of the
	/// pointed-to element within the container.
	ReturnType operator *() const { return d_->at(i_); }
	
	/// \brief Return a copy of the element at the given position
	/// within the container.
	ReturnType operator [](int n) const { return d_->at(n); }
	
	/// \brief Move the iterator to the next element, returning an
	/// iterator to that element
	subscript_iterator& operator ++() { ++i_; return *this; }

	/// \brief Move the iterator to the next element, returning an
	/// iterator to the element we were pointing at before the change
	subscript_iterator operator ++(int)
	{
		subscript_iterator tmp = *this;
		++i_;
		return tmp;
	}

	/// \brief Move the iterator to the previous element, returning an
	/// iterator to that element
	subscript_iterator& operator --()
	{
		--i_;
		return *this;
	}

	/// \brief Move the iterator to the previous element, returning an
	/// iterator to the element we were pointing at before the change
	subscript_iterator operator --(int)
	{
		subscript_iterator tmp = *this;
		--i_;
		return tmp;
	}

	/// \brief Advance iterator position by \c n
	subscript_iterator& operator +=(SizeType n)
	{
		i_ += n;
		return *this;
	}

	/// \brief Return an iterator \c n positions beyond this one
	subscript_iterator operator +(SizeType n) const
	{
		subscript_iterator tmp = *this;
		tmp.i_ += n;
		return tmp;
	}
	
	/// \brief Move iterator position back by \c n
	subscript_iterator& operator -=(SizeType n)
	{
		i_ -= n;
		return *this;
	}

	/// \brief Return an iterator \c n positions before this one
	subscript_iterator operator -(SizeType n) const
	{
		subscript_iterator tmp = *this;
		tmp.i_ -= n;
		return tmp;
	}
	
	/// \brief Return an iterator \c n positions before this one
	DiffType operator -(const subscript_iterator& j) const
	{
		if (d_ == j.d_) {
			return static_cast<SizeType>(i_) - j.i_;
		}
		return 0;
	}

private:
	SizeType i_;
	OnType* d_;
};


/// \brief Add a constant to a subscript_iterator, returning a
/// subscript_iterator offset by that amount
template <class OnType, class ReturnType, class SizeType,
		class DiffType> 
inline subscript_iterator<OnType, ReturnType, SizeType, DiffType>
operator +(SizeType x,
		const subscript_iterator <OnType, ReturnType, SizeType, DiffType>& y)
{
	return y + x;
}

} // end namespace mysqlpp

#endif
