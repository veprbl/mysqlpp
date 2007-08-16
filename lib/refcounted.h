/// \file refcounted.h
/// \brief Declares the RefCountedPointer template

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

#endif // !defined(MYSQLPP_REFCOUNTED_H)

