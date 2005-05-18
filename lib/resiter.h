/// \file resiter.h
/// \brief Declares templates for adapting existing classes to
/// be iteratable random-access containers.
///
/// The file name seems to tie it to the mysqlpp::Result class, which
/// is so adapted, but these templates are also used to adapt the
/// mysqlpp::Fields and mysqlpp::Row classes.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
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

#ifndef MYSQLPP_RESITER_H
#define MYSQLPP_RESITER_H

#include "defs.h"

#include <iterator>

namespace mysqlpp {

template <class OnType, class ReturnType, class SizeType, class DiffType>
class subscript_iterator;

/// \brief A base class that one derives from to become a random
/// access container, which can be accessed with subscript notation.
///
/// OnType must have the member functions \c operator[](SizeType) and
// \c size() defined for it.

template <class OnType, class ValueType, class ReturnType = const ValueType&, class SizeType = unsigned int, class DiffType = int>
class const_subscript_container {
public:
  typedef const_subscript_container<OnType,ValueType,ReturnType,SizeType,DiffType>
		this_type;

  typedef subscript_iterator<const this_type, ReturnType, SizeType, DiffType> 
		iterator;
  typedef iterator const_iterator;
  typedef const std::reverse_iterator<iterator> reverse_iterator;
  typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;
  
  typedef ValueType value_type;
  typedef value_type& reference;
  typedef value_type& const_reference;
  typedef value_type* pointer;
  typedef value_type* const_pointer;
  
  typedef DiffType difference_type;
  typedef SizeType size_type;
 
  virtual size_type  size() const = 0;
  virtual ReturnType operator[] (SizeType i) const = 0;

  size_type max_size() const { return size(); }
  bool empty() const { return size()==0; }
  
  iterator begin() const { return iterator(this, 0); }
  iterator end() const { return iterator(this, size()); }
  
  reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() const { return reverse_iterator(begin()); }
};


/// \brief Iterator that can be subscripted.
///
/// This is the type of iterator used by the const_subscript_container
/// template.

template <class OnType, class ReturnType, class SizeType, class DiffType>
class subscript_iterator : public std::iterator<ReturnType, SizeType>
{
private:
  SizeType    i;
  OnType     *d;
public:
  subscript_iterator() {}; 
  subscript_iterator(OnType *what, SizeType pos) {d=what; i=pos;}
 
  bool operator == (const subscript_iterator &j) const
    {if (d == j.d && i==j.i) return true; return false;}
  bool operator != (const subscript_iterator &j) const
    {if (d == j.d && i!=j.i) return true; return false;}
  bool operator < (const subscript_iterator &j) const
    {if (d == j.d && i < j.i) return true; return false;}
  bool operator > (const subscript_iterator &j) const
    {if (d == j.d && i > j.i) return true; return false;}
  bool operator <= (const subscript_iterator &j) const
    {if (d == j.d && i<=j.i) return true; return false;}
  bool operator >= (const subscript_iterator &j) const
    {if (d == j.d && i>=j.i) return true; return false;}

  ReturnType* operator -> () const {return &((*d)[i]);}
  ReturnType operator *  () const {return (*d)[i];}
  ReturnType operator [] (SizeType n) const {return (*d)[n];}

  subscript_iterator& operator ++ () {i++; return *this;}
  subscript_iterator  operator ++ (int) 
    {subscript_iterator tmp = *this; i++; return tmp;}
  subscript_iterator& operator -- () {i--; return *this;}
  subscript_iterator  operator -- (int) 
    {subscript_iterator tmp = *this; i--; return tmp;}
  subscript_iterator& operator += (SizeType n) {i=i+n; return *this;}
  subscript_iterator  operator + (SizeType n) const
    {subscript_iterator tmp = *this; tmp.i+=n; return tmp;}
  subscript_iterator& operator -= (SizeType n) {i=i-n; return *this;}
  subscript_iterator  operator - (SizeType n) const
    {subscript_iterator tmp = *this; tmp.i-=n; return tmp;}
  DiffType operator - (const subscript_iterator &j) const
    {if (d == j.d) return static_cast<SizeType>(i) - j.i; return 0;}
};


/// \if INTERNAL
// Doxygen will not generate documentation for this section.

template <class OnType, class ReturnType, class SizeType, class DiffType>
inline subscript_iterator<OnType,ReturnType,SizeType,DiffType>
operator +(SizeType x,
		const subscript_iterator<OnType, ReturnType, SizeType, DiffType>& y) 
{
  return y + x;
}

/// \endif

} // end namespace mysqlpp

#endif

