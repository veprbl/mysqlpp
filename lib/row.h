/// \file row.h
/// \brief Declares the classes for holding row data from a result set.

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

#ifndef MYSQLPP_ROW_H
#define MYSQLPP_ROW_H

#include "common.h"

#include "mystring.h"
#include "noexceptions.h"
#include "refcounted.h"
#include "subiter.h"
#include "vallist.h"

#include <vector>
#include <string>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class FieldNames;
class MYSQLPP_EXPORT ResUse;
#endif

/// \brief Manages rows from a result set.
class MYSQLPP_EXPORT Row : public OptionalExceptions
{
public:
	typedef int difference_type;			///< type for index differences
	typedef unsigned int size_type;			///< type of returned sizes
	typedef String value_type;				///< type of data in container

	/// \brief regular iterator type
	///
	/// Note that this is the same as const_iterator; we don't have a
	/// mutable iterator type.
	typedef subscript_iterator<const Row, const value_type, size_type,
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
	Row() :
	initialized_(false),
	size_(0)
	{
	}
	
	/// \brief Copy constructor
	Row(const Row& r) :
	data_(r.data_.begin(), r.data_.end()),
	field_names_(r.field_names_),
	initialized_(r.initialized_),
	size_(r.size_)
	{
	}

	/// \brief Create a row object
	///
	/// \param d MySQL C API row data
	/// \param r result set that the row comes from
	/// \param lengths length of each item in d
	/// \param te if true, throw exceptions on errors
	Row(MYSQL_ROW d, const ResUse* r, unsigned long* lengths, bool te = true);

	/// \brief Destroy object
	~Row() { }

	/// \brief Get the number of fields in the row.
	size_type size() const { return size_; }

	/// \brief Assignment operator
	Row& operator=(const Row& rhs)
	{
		data_.assign(rhs.data_.begin(), rhs.data_.end());
		field_names_.assign(rhs.field_names_);
		initialized_ = rhs.initialized_;
		size_ = rhs.size_;
		return *this;
	}

	/// \brief Get the value of a field given its name.
	///
	/// If the field does not exist in this row, we throw a BadFieldName
	/// exception.
	///
	/// This operator is fairly inefficient.  operator[](int) is faster.
	const value_type& operator [](const char* field) const;

	/// \brief Get the value of a field given its index.
	///
	/// This function is just syntactic sugar, wrapping the at() method.
	const value_type& operator [](int i) const { return at(i); }

	/// \brief Get the value of a field given its index.
	///
	/// If the index value is bad, the underlying std::vector is
	/// supposed to throw an exception, according to the Standard.
	const value_type& at(int i) const { return data_.at(i); }

	/// \brief Returns true if there is data in the row.
	operator bool() const { return data_.size(); }

	/// \brief Returns a field's index given its name
	size_type field_num(const char* name) const;

	/// \brief Get a list of the values in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, and the quoting and escaping rules will be
	/// set by the manipulator 'm' you choose.
	///
	/// \param d delimiter to use between values
	/// \param m manipulator to use when inserting values into a stream
	template <class Manip>
	value_list_ba<Row, Manip> value_list(const char* d = ",",
			Manip m = quote) const
	{
		return value_list_ba<Row, Manip>(*this, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param d delimiter to use between values
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	/// \param m manipulator to use when inserting values into a stream
	template <class Manip>
	value_list_b<Row, Manip> value_list(const char *d,
			const std::vector<bool>& vb, Manip m = quote) const
	{
		return value_list_b<Row, Manip>(*this, vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	///
	/// Items will be quoted and escaped when inserted into a C++ stream,
	/// and a comma will be used as a delimiter between the items.
	value_list_b<Row, quote_type0> value_list(
			const std::vector<bool> &vb) const
	{
		return value_list_b<Row, quote_type0>(*this, vb, ",", quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the delimiter 'd' will be placed between the
	/// items, and the manipulator 'm' used before each item.
	template <class Manip>
	value_list_b<Row, Manip> value_list(const char *d, Manip m,
			bool t0, bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<Row, Manip>(*this, vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the delimiter 'd' will be placed between the
	/// items, and items will be quoted and escaped.
	value_list_b <Row, quote_type0>
	value_list(const char *d, bool t0, bool t1 = false, bool t2 = false,
			bool t3 = false, bool t4 = false, bool t5 = false,
			bool t6 = false, bool t7 = false, bool t8 = false,
			bool t9 = false, bool ta = false, bool tb = false,
			bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<Row, quote_type0>(*this, vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the a comma will be placed between the items,
	/// as a delimiter, and items will be quoted and escaped.
	value_list_b<Row, quote_type0> value_list(bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<Row, quote_type0>(*this, vb, ",", quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, the delimiter
	/// 'd' will be placed between the items, and the manipulator 'm'
	/// will be inserted before each item.
	template <class Manip>
	value_list_b<Row, Manip> value_list(const char *d, Manip m,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(*this, vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<Row, Manip>(*this, vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, the delimiter
	/// 'd' will be placed between the items, and items will be quoted
	/// and escaped.
	value_list_b<Row, quote_type0> value_list(
			const char *d,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(*this, vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<Row, quote_type0>(*this, vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, a comma will be
	/// placed between the items as a delimiter, and items will be
	/// quoted and escaped.
	value_list_b<Row, quote_type0> value_list(
			std::string s0,
			std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(*this, vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<Row, quote_type0>(*this, vb, ",", quote);
	}

	/// \brief Get a list of the field names in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, and no manipulator will be used on the items.
	value_list_ba<FieldNames, do_nothing_type0>
			field_list(const char* d = ",") const;

	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param m manipulator to use before each item when the list is
	/// inserted into a C++ stream
	template <class Manip>
	value_list_ba<FieldNames, Manip> field_list(const char* d,
			Manip m) const;

	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param m manipulator to use before each item when the list is
	/// inserted into a C++ stream
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	template <class Manip>
	value_list_b<FieldNames, Manip> field_list(const char* d, Manip m,
			const std::vector<bool>& vb) const;

	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	///
	/// Field names will be quoted and escaped when inserted into a C++
	/// stream.
	value_list_b<FieldNames, quote_type0> field_list(
			const char* d, const std::vector<bool>& vb) const;

	/// \brief Get a list of the field names in this row
	///
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	///
	/// Field names will be quoted and escaped when inserted into a C++
	/// stream, and a comma will be placed between them as a delimiter.
	value_list_b<FieldNames, quote_type0> field_list(
			const std::vector<bool>& vb) const;

	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, the delimiter 'd' will be placed
	/// between the items as a delimiter, and the manipulator 'm' used
	/// before each item.
	template <class Manip>
	value_list_b<FieldNames, Manip> field_list(const char *d, Manip m,
			bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const;

	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, the delimiter 'd' will be placed
	/// between the items as a delimiter, and the items will be quoted
	/// and escaped.
	value_list_b<FieldNames, quote_type0> field_list(
			const char *d, bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const;

	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, a comma will be placed between the
	/// items as a delimiter, and the items will be quoted and escaped.
	value_list_b<FieldNames, quote_type0> field_list(
			bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const;

	/// \brief Get an "equal list" of the fields and values in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, " = " is the relationship operator, and items
	/// will be quoted and escaped.
	equal_list_ba<FieldNames, Row, quote_type0>
			equal_list(const char* d = ",", const char* e = " = ") const;

	/// \brief Get an "equal list" of the fields and values in this row
	///
	/// This method's parameters govern how the returned list will
	/// behave when you insert it into a C++ stream:
	///
	/// \param d delimiter to use between items
	/// \param e the operator to use between elements
	/// \param m the manipulator to use for each element
	///
	/// For example, if d is ",", e is " = ", and m is the quote
	/// manipulator, then the field and value lists (a, b) (c, d'e)
	/// will yield an equal list that gives the following when inserted
	/// into a C++ stream:
	///
	/// \code
	///   'a' = 'c', 'b' = 'd''e'
	/// \endcode
	///
	/// Notice how the single quote was 'escaped' in the SQL way to
	/// avoid a syntax error.
	template <class Manip>
	equal_list_ba<FieldNames, Row, Manip> equal_list(const char* d,
			const char* e, Manip m) const;

private:
	std::vector<value_type> data_;
	RefCountedPointer<FieldNames> field_names_;
	bool initialized_;
	size_type size_;
};

} // end namespace mysqlpp

#endif
