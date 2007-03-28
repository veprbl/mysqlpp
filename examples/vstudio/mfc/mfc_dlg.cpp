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

#include "mfc_dlg.h"
#include "util.h"

#include <mysql++.h>

BEGIN_MESSAGE_MAP(CExampleDlg, CDialog)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CExampleDlg::OnBnClickedConnectButton)
END_MESSAGE_MAP()


//// ctor //////////////////////////////////////////////////////////////

CExampleDlg::CExampleDlg(CWnd* pParent) :
CDialog(CExampleDlg::IDD, pParent),
hIcon_(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
{
	// Load default input values from registry, if we can
	HKEY key = OpenSettingsRegistryKey();
	if (key) {
		// There are pre-existing defaults we can use, so copy them in
		TCHAR acSetting[100];
		LoadSetting(key, _T("user"), acSetting, sizeof(acSetting));
		sUserName = acSetting;
		LoadSetting(key, _T("server"), acSetting, sizeof(acSetting));
		sServerAddress = acSetting;
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


//// OnBnClickedConnectButton //////////////////////////////////////////
// This is essentially the same thing as examples/simple1.cpp

void
CExampleDlg::OnBnClickedConnectButton()
{
	WCHAR awcTempBuf[100];
	const int kTempBufSize = sizeof(awcTempBuf) / sizeof(awcTempBuf[0]);

	// Pull user input into our member variables, then save that to the
	// registry for future use.
	UpdateData(TRUE);
	SaveInputs(sServerAddress, sUserName);

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
