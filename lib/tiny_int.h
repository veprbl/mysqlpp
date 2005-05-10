/// \file tiny_int.h
/// \brief Declares class for holding a SQL tiny_int

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

#ifndef MYSQLPP_TINY_INT_H
#define MYSQLPP_TINY_INT_H

namespace mysqlpp {

/// \brief Class for holding an SQL \c tiny_int object.
///
/// This is required because the closest C++ type, \c char, doesn't
/// have all the right semantics.  For one, inserting a \c char into a
/// stream won't give you a number.

class tiny_int {
  char value;
public:
  tiny_int() {}
  tiny_int(short int v) : value(char(v)) {}
  operator short int() const {return static_cast<short int>(value);};
  tiny_int &operator = (short int v) {value = char(v); return *this;}
  tiny_int &operator += (short int v) {value += char(v); return *this;}
  tiny_int &operator -= (short int v) {value -= char(v); return *this;}
  tiny_int &operator *= (short int v) {value *= char(v); return *this;}
  tiny_int &operator /= (short int v) {value /= char(v); return *this;}
  tiny_int &operator %= (short int v) {value %= char(v); return *this;}
  tiny_int &operator &= (short int v) {value &= char(v); return *this;}
  tiny_int &operator |= (short int v) {value |= char(v); return *this;}
  tiny_int &operator ^= (short int v) {value ^= char(v); return *this;}
  tiny_int &operator <<= (short int v) {value <<= char(v); return *this;}
  tiny_int &operator >>= (short int v) {value >>= char(v); return *this;}
  tiny_int &operator ++ () {value++; return *this;}
  tiny_int &operator -- () {value--; return *this;}
  tiny_int operator ++ (int) {tiny_int tmp=value; value++; return tmp;}
  tiny_int operator -- (int) {tiny_int tmp=value; value--; return tmp;}
  tiny_int operator - (const tiny_int& i) const {return value - i;}
  tiny_int operator + (const tiny_int& i) const {return value + i;}
  tiny_int operator * (const tiny_int& i) const {return value * i;}
  tiny_int operator / (const tiny_int& i) const {return value / i;}
  tiny_int operator % (const tiny_int& i) const {return value % i;}
  tiny_int operator | (const tiny_int& i) const {return value | i;}
  tiny_int operator & (const tiny_int& i) const {return value & i;}
  tiny_int operator ^ (const tiny_int& i) const {return value ^ i;}
  tiny_int operator << (const tiny_int& i) const {return value << i;}
  tiny_int operator >> (const tiny_int& i) const {return value >> i;}
};

} // end namespace mysqlpp

#endif

