/***********************************************************************
 util.cpp - Functions used by all of the Visual C++ examples.  If you're
	just trying to understand the examples, you can pretty much just
	ignore this module and depend on the functions' names: they do what
	they're called. :)

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

#include "util.h"


//// LoadSetting ///////////////////////////////////////////////////////
// Loads up the value of the named registry value underneath the given
// key and returns it in pcValue.

bool
LoadSetting(HKEY key, LPCTSTR pcName, LPTSTR pcValue, DWORD nValueSize)
{
	return RegQueryValueEx(key, pcName, 0, 0, LPBYTE(pcValue),
			&nValueSize) == ERROR_SUCCESS;
}


//// OpenSettingsRegistryKey ///////////////////////////////////////////

HKEY
OpenSettingsRegistryKey()
{
	HKEY key1, key2;
	if ((RegOpenKey(HKEY_CURRENT_USER, _T("Software"), &key1) ==
			ERROR_SUCCESS) && (RegCreateKey(key1,
			_T("MySQL++ Examples"), &key2) == ERROR_SUCCESS)) {
		RegCloseKey(key1);
		return key2;
	}
	else {
		return 0;
	}
}


//// SaveInputs ////////////////////////////////////////////////////////
// Save the given strings to the registry so future runs and other
// examples can use them.

bool
SaveInputs(LPCTSTR pcServerAddress, LPCTSTR pcUserName)
{
	HKEY key = OpenSettingsRegistryKey();
	if (key) {
		SaveSetting(key, _T("user"), pcUserName);
		SaveSetting(key, _T("server"), pcServerAddress);
		RegCloseKey(key);
		return true;
	}
	else {
		return false;
	}
}


//// SaveSetting ///////////////////////////////////////////////////////
// Saves the given value as a named entry under the given registry key.

bool
SaveSetting(HKEY key, LPCTSTR pcName, LPCTSTR pcValue)
{
	DWORD nBytes = DWORD(sizeof(TCHAR) * (_tcslen(pcValue) + 1));
	return RegSetValueEx(key, pcName, 0, REG_SZ, LPBYTE(pcValue),
			nBytes) == ERROR_SUCCESS;
}


//// ToUCS2 ////////////////////////////////////////////////////////////
// Convert a C string in UTF-8 format to UCS-2 format.

bool
ToUCS2(LPTSTR pcOut, int nOutLen, const char* kpcIn)
{
	if (strlen(kpcIn) > 0) {
		// Do the conversion normally
		return MultiByteToWideChar(CP_UTF8, 0, kpcIn, -1, pcOut,
				nOutLen) > 0;
	}
	else if (nOutLen > 1) {
		// Can't distinguish no bytes copied from an error, so handle
		// an empty input string as a special case.
		_tccpy(pcOut, _T(""));
		return true;
	}
	else {
		// Not enough room to do anything!
		return false;
	}
}


//// ToUTF8 ////////////////////////////////////////////////////////////
// Convert a UCS-2 multibyte string to the UTF-8 format required by
// MySQL, and thus MySQL++.

bool
ToUTF8(char* pcOut, int nOutLen, LPCWSTR kpcIn)
{
	if (_tcslen(kpcIn) > 0) {
		// Do the conversion normally
		return WideCharToMultiByte(CP_UTF8, 0, kpcIn, -1, pcOut,
				nOutLen, 0, 0) > 0;
	}
	else if (nOutLen > 0) {
		// Can't distinguish no bytes copied from an error, so handle
		// an empty input string as a special case.
		*pcOut = '\0';
		return true;
	}
	else {
		// Not enough room to do anything!
		return false;
	}
}

