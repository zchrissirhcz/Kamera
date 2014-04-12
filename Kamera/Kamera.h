// Detektor DCC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "CameraDialog.h"

// CDetektorDCCApp:
// See Detektor DCC.cpp for the implementation of this class
//

class CDetektorDCCApp : public CWinApp
{
public:
	CDetektorDCCApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDetektorDCCApp theApp;