/***********************************************************************
 util.h - Declares the utility functions used by all of the Visual C++
 	examples.

 Copyright (c) 2007 by Educational Technology Resources, Inc.  Others 
 may also hold copyrights on code in this file.  See the CREDITS file in
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

#pragma once

bool LoadSetting(HKEY key, LPCTSTR pcName, LPTSTR pcValue,
		DWORD nValueSize);
HKEY OpenSettingsRegistryKey();
bool SaveInputs(LPCTSTR pcServerAddress, LPCTSTR pcUserName);
bool SaveSetting(HKEY key, LPCTSTR pcName, LPCTSTR pcValue);
bool ToUCS2(LPTSTR pcOut, int nOutLen, const char* kpcIn);
bool ToUTF8(char* pcOut, int nOutLen, LPCWSTR kpcIn);

