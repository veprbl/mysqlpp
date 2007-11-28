/// \file refcounted.h
/// \brief Declares the RefCountedPointer template and RefCountedBuffer
/// class

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

#if !defined(MYSQLPP_REFCOUNTED_H)
#define MYSQLPP_REFCOUNTED_H

#include "type_info.h"

#include <string>

namespace mysqlpp {

/// \brief Creates an object that acts as a reference-counted pointer
/// to another object.
///
/// Resulting type acts like a pointer in all respects, except that it
/// manages the memory it points to by observing how many users there
/// are for the object.
///
/// This attempts to be as automatic as reference counting in a
/// programming language with memory management.  Like all automatic
/// memory management schemes, it has penalties: it turns the single
/// indirection of an unmanaged pointer into a double indirection, and
/// has additional management overhead in the assignment operators due
/// to the reference counter.  This is an acceptable tradeoff when
/// wrapping objects that are expensive to copy, and which need to
/// be "owned" by disparate parties: you can allocate the object just
/// once, then pass around the reference counted pointer, knowing that
/// the last user will "turn out the lights".
///
/// \b Implementation \b detail: You may notice that this class manages
/// two pointers, one to the data we're managing, and one to the
/// reference count.  You might wonder why we don't wrap these up into a
/// structure and keep just a pointer to an instance of it to simplify
/// the memory management.  It would indeed do that, but then every
/// access to the data we manage would be a triple indirection instead
/// of just double.  It's a tradeoff, and we've chosen to take a minor
/// complexity hit to avoid the performance hit.

template <class T>
class RefCountedPointer
{
public:
	typedef RefCountedPointer<T> ThisType;	///< alias for this object's type
	
	/// \brief Standard constructor
	///
	/// \param c A pointer to the managed object.  If you leave it at
	/// its default (or pass 0 explicitly) this object is useless until
	/// you vivify it with operator =() or assign().
	explicit RefCountedPointer(T* c = 0) :
	counted_(c),
	refs_(c ? new size_t(1) : 0)
	{
	}

	/// \brief Copy constructor
	RefCountedPointer(const ThisType& other) :
	counted_(0),		// yes, this is required! see assign()
	refs_(0)
	{
		assign(other);
	}

	/// \brief Destructor
	///
	/// This only destroys the managed memory if the reference count
	/// drops to 0.
	~RefCountedPointer()
	{
		detach();
	}

	/// \brief Sets (or resets) the pointer to the counted object.
	///
	/// Before we do the actual assignment, we decrement the previous
	/// managed object's refcount.  If it falls to 0, it's destroyed.
	///
	/// If the refcount is >= 2 when we are called, the other users of
	/// the managed memory will see no difference; the internal refcount
	/// will just be 1 lower, which is imperceptible from the outside.
	/// Only this object sees a change in what's being pointed to.
	void assign(T* c)
	{
		detach();

		if (c) {
			counted_ = c;
			refs_ = new size_t(1);
		}
		else {
			counted_ = 0;
			refs_ = 0;
		}
	}

	/// \brief Copy an existing refcounted pointer
	///
	/// This just copies the pointer to the underlying counted pointer
	/// and increases the reference count.
	///
	/// If this object already pointed to managed memory, we decrement
	/// the refcount for it first, and destroy the managed memory block
	/// if the refcount falls to 0.
	void assign(const ThisType& other)
	{
		detach();
		
		if (other.counted_ && other.refs_) {
			counted_ = other.counted_;
			refs_ = other.refs_;
			++(*refs_);
		}
		else {
			counted_ = 0;
			refs_ = 0;
		}
	}

	/// \brief Set (or reset) the pointer to the counted object
	///
	/// This is essentially the same thing as assign(T*).  The
	/// choice between the two is just a matter of syntactic preference.
	ThisType& operator =(T* c)
	{
		assign(c);
		return *this;
	}

	/// \brief Copy an existing refcounted pointer
	///
	/// This is essentially the same thing as assign(const ThisType&).
	/// The choice between the two is just a matter of syntactic
	/// preference.
	ThisType& operator =(const ThisType& rhs)
	{
		assign(rhs);
		return *this;
	}

	/// \brief Returns true if other refcounted pointer's internal
	/// data pointer is the same as this one's.
	bool operator ==(const ThisType& rhs)
	{
		return rhs->counted_ == counted_;
	}

	/// \brief Access the object through the smart pointer
	T* operator ->() const
	{
		return counted_;
	}	

	/// \brief Dereference the smart pointer
	T& operator *() const
	{
		return *counted_;
	}	

	/// \brief Returns true if this object can be dereferenced.
	operator bool() const
	{
		return counted_ != 0;
	}

	/// \brief Returns true if this object cannot be safely dereferenced
	bool operator !() const
	{
		return counted_ == 0;
	}

private:
	/// \brief Detach ourselves from the managed memory block.
	///
	/// If we are managing memory, decreases the reference count and
	/// destroys the memory if the counter falls to 0.
	void detach()
	{
		if (refs_ && (--(*refs_) == 0)) {
			delete counted_;
			delete refs_;
			counted_ = 0;
			refs_ = 0;
		}
	}
	
	/// \brief Pointer to the reference-counted object
	T* counted_;

	/// \brief Pointer to the reference count.
	///
	/// We can't keep this as a plain integer because this object
	/// allows itself to be copied.  All copies need to share this
	/// reference count, not just the pointer to the counted object.
	size_t* refs_;
};


/// \brief Holds a reference-counted data buffer, like a primitive
/// sort of std::string.
class RefCountedBuffer
{
public:
	/// \brief Type of length values
	typedef unsigned int size_type;

	/// \brief Initialize object as a copy of a raw data buffer
	///
	/// Copies the string into a new buffer one byte longer than
	/// the length value given, using that to hold a C string null
	/// terminator, just for safety.  The length value we keep does
	/// not include this extra byte, allowing this same mechanism
	/// to work for both C strings and binary data.
	RefCountedBuffer(const char* data, size_type length,
			mysql_type_info type, bool is_null)
			{ init(data, length, type, is_null); }
 
	/// \brief Initialize object as a copy of a C++ string object
	RefCountedBuffer(const std::string& s, mysql_type_info type,
			bool is_null) { init(s.data(), s.length(), type, is_null); }

	/// \brief Destructor
	~RefCountedBuffer() { delete[] data_; }

	/// \brief Replace contents of buffer with copy of given C string
	RefCountedBuffer& assign(const char* data, size_type length,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false);

	/// \brief Replace contents of buffer with copy of given C++ string
	RefCountedBuffer& assign(const std::string& s,
			mysql_type_info type = mysql_type_info::string_type,
			bool is_null = false);

	/// \brief Increment reference count
	void attach() { ++refs_; }

	/// \brief Return pointer to raw data buffer
	const char* data() const { return data_; }

	/// \brief Decrement reference count
	/// \return True if reference count has not yet fallen to zero
	bool detach() { return --refs_ > 0; }

	/// \brief Returns true if we were initialized with a data type
	/// that must be escaped when used in a SQL query
	bool escape_q() const { return type_.escape_q(); }

	/// \brief Return number of bytes in data buffer
	///
	/// Count does not include the trailing null we tack on to our
	/// copy of the buffer for ease of use in C string contexts.
	/// We do this because we can be holding binary data just as
	/// easily as a C string.
	size_type length() const { return length_; }

	/// \brief Returns true if type of buffer's contents is string
	bool is_string() { return type_ == mysql_type_info::string_type; }

	/// \brief Return true if buffer's contents represent a SQL
	/// null.
	///
	/// The buffer's actual content will probably be "NULL" or
	/// something like it, but in the SQL data type system, a SQL
	/// null is distinct from a plain string with value "NULL".
	bool is_null() const { return is_null_; }

	/// \brief Returns true if we were initialized with a data type
	/// that must be quoted when used in a SQL query
	bool quote_q() const { return type_.quote_q(); }

	/// \brief Sets the internal SQL null flag
	void set_null() { is_null_ = true; }

	/// \brief Return the SQL type of the data held in the buffer
	const mysql_type_info& type() const { return type_; }

private:
	/// \brief Common initialization for ctors
	void init(const char* pd, size_type len, mysql_type_info type,
			bool is_null);
	/// \brief Implementation detail of assign() and init()
	void replace_buffer(const char* pd, size_type length);

	const char* data_;		///< pointer to the raw data buffer
	size_type length_;		///< bytes in buffer, without trailing null
	mysql_type_info type_;	///< SQL type of data in the buffer
	bool is_null_;			///< if true, string represents a SQL null
	unsigned int refs_;		///< reference count for this object
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_REFCOUNTED_H)

