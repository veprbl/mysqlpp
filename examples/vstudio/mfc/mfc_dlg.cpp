/***********************************************************************
 mfc_dlg.cpp - Defines the dialog box behavior for the MySQL++ MFC
	example.

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

#include "stdafx.h"
#include "mfc.h"
#include "mfc_dlg.h"

#include <mysql++.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NELEMS(s) (sizeof(s) / sizeof(s[0]))

BEGIN_MESSAGE_MAP(CExampleDlg, CDialog)
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CExampleDlg::OnBnClickedConnectButton)
END_MESSAGE_MAP()


//// ctor //////////////////////////////////////////////////////////////

CExampleDlg::CExampleDlg(CWnd* pParent) :
CDialog(CExampleDlg::IDD, pParent)
{
	// Load default input values from registry, if we can
	HKEY key = OpenSettingsRegistryKey();
	if (key) {
		// There are pre-existing defaults we can use, so copy them in
		LoadSetting(key, _T("user"), sUserName);
		LoadSetting(key, _T("server"), sServerAddress);
		RegCloseKey(key);
	}
	if ((sUserName.GetLength() == 0) || 
			(sServerAddress.GetLength() == 0)) {
		// Didn't find anything we can use, so have to make something
		// plausible up instead.
		sServerAddress = _T("localhost");

		TCHAR acUserName[100];
		DWORD nBufferSize = sizeof(acUserName);
		if (GetUserName(acUserName, &nBufferSize)) {
			sUserName = acUserName;
		}
	}

	hIcon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


//// AddMessage ////////////////////////////////////////////////////////
// Inserts the given string at the end of the list box we're using for
// output to the user.

void 
CExampleDlg::AddMessage(LPCTSTR pcMessage)
{
	ResultsList.InsertString(-1, pcMessage);
}


//// DoDataExchange ////////////////////////////////////////////////////
// Transfer data from the controls into our member variables

void CExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVER_EDIT, sServerAddress);
	DDX_Text(pDX, IDC_USER_EDIT, sUserName);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, sPassword);
	DDX_Control(pDX, IDC_RESULTS_LIST, ResultsList);
}


//// LoadSetting ///////////////////////////////////////////////////////
// Loads up the value of the named registry value underneath the given
// key and returns it in sValue.

bool
CExampleDlg::LoadSetting(HKEY key, LPCTSTR pcName, CString& sValue)
{
	TCHAR acBuffer[100];
	DWORD nBufSize = sizeof(acBuffer);
	if (RegQueryValueEx(key, pcName, 0, 0, LPBYTE(acBuffer),
			&nBufSize) == ERROR_SUCCESS) {
		sValue = acBuffer;
		return true;
	}
	else {
		return false;
	}
}


//// OnBnClickedConnectButton //////////////////////////////////////////
// This is essentially the same thing as examples/simple1.cpp

void
CExampleDlg::OnBnClickedConnectButton()
{
	WCHAR awcTempBuf[100];
	const int kTempBufSize = NELEMS(awcTempBuf);

	// Save the user inputs to our member variables, and as a side
	// effect, to the registry for future use.
	SaveInputs();

	// Clear out the results list, in case this isn't the first time
	// we've come in here.
	ResultsList.ResetContent();

	// Translate the Unicode text we get from the UI into the UTF-8 form
	// that MySQL wants.
	const int kInputBufSize = 100;
	char acServerAddress[kInputBufSize];
	char acUserName[kInputBufSize];
	char acPassword[kInputBufSize];
	ToUTF8(acServerAddress, kInputBufSize, sServerAddress);
	ToUTF8(acUserName, kInputBufSize, sUserName);
	ToUTF8(acPassword, kInputBufSize, sPassword);

	// Connect to the sample database.
	mysqlpp::Connection con(false);
	if (!con.connect("mysql_cpp_data", acServerAddress, acUserName,
			acPassword)) {
		AddMessage(_T("Failed to connect to server:"));
		if (ToUCS2(awcTempBuf, kTempBufSize, con.error())) {
			AddMessage(awcTempBuf);
		}
		return;
	}

	// Retrieve a subset of the sample stock table set up by resetdb
	mysqlpp::Query query = con.query();
	query << "select item from stock";
	mysqlpp::Result res = query.store();

	// Display the result set
	if (res) {
		mysqlpp::Row row;
		for (mysqlpp::Row::size_type i = 0; row = res.at(i); ++i) {
			if (ToUCS2(awcTempBuf, kTempBufSize, row.at(0))) {
				AddMessage(awcTempBuf);
			}
		}
	}
	else {
		AddMessage(_T("Failed to get item list:"));
		if (ToUCS2(awcTempBuf, kTempBufSize, query.error().c_str())) {
			AddMessage(awcTempBuf);
		}
	}
}


//// OnInitDialog //////////////////////////////////////////////////////

BOOL 
CExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(hIcon_, TRUE);
	SetIcon(hIcon_, FALSE);

	return TRUE;
}


//// OpenSettingsRegistryKey ///////////////////////////////////////////

HKEY
CExampleDlg::OpenSettingsRegistryKey()
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

void CExampleDlg::SaveInputs()
{
	// Kick DDX code, make it give us current data
	UpdateData(TRUE);

	// Save that data to the registry so future runs and other examples
	// can use it.
	HKEY key = OpenSettingsRegistryKey();
	if (key) {
		SaveSetting(key, _T("user"), sUserName);
		SaveSetting(key, _T("server"), sServerAddress);
		RegCloseKey(key);
	}
}


//// SaveSetting ///////////////////////////////////////////////////////
// Saves the given value as a named entry under the given registry key.

void
CExampleDlg::SaveSetting(HKEY k, LPCTSTR pcName, const CString& sValue)
{
	RegSetValueEx(k, pcName, 0, REG_SZ, (const BYTE*)(LPCTSTR)(sValue),
			sizeof(TCHAR) * (sValue.GetLength() + 1));
}


//// ToUCS2 ////////////////////////////////////////////////////////////
// Convert a C string in UTF-8 format to UCS-2 format.

bool CExampleDlg::ToUCS2(LPTSTR pcOut, int nOutLen,
		const char* kpcIn)
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
CExampleDlg::ToUTF8(char* pcOut, int nOutLen, LPCWSTR kpcIn)
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
